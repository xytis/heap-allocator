/*
 * =====================================================================================
 *
 *       Filename:  TypeTracking.hpp
 *
 *    Description:  Utilities for type tracking
 *
 *        Version:  1.0
 *        Created:  02/09/2013 11:25:57 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Vytis Valentinavičius (), vytis.valentinavicius@gmail.com
 *        Company:  VU FF
 *
 * =====================================================================================
 */


#ifndef __type_tracking_hpp__
#define __type_tracking_hpp__

#include <vector>
#include <iostream>
#include <iomanip>
#include <typeinfo>

/**
 * Interface for typed tracking info.
 */
class TrackingInfo {
protected:
  typedef size_t        size_type;
  
  TrackingInfo() {};
  TrackingInfo(const TrackingInfo & copy) = delete;
  virtual ~TrackingInfo() {}; //Could be made purely virtual, but I need seperate
                              //implementation file for such action.
protected:
  friend class GlobalTracker;
  
  virtual void report_statistics(std::ostream & output) = 0;
  virtual void note_allocation(size_type amount) = 0;
  virtual void note_deallocation(size_type amount) = 0;
  virtual void reset() = 0;
};


/**
 * Holder for all types that exist in the system and are suitable for tracking.
 */
class GlobalTracker {
public:
  static void track(TrackingInfo * info) {
    if (!pointer_) { construct(); }
    pointer_->save_trackable(info);
  }
  
  static void report(std::ostream & output) {
    if (!pointer_) { construct(); }
    pointer_->report_statistics(output);
  }
  
  static void clean() {
    if (!pointer_) { construct(); }
    pointer_->clean_trackables();
  }
protected:
  static void construct() {
    if (pointer_) { return; }
    else {
      //Start thread unsafe code:
      pointer_ = new GlobalTracker();
      //End thread unsafe code.
    }
  }
  
  GlobalTracker() {};
  GlobalTracker(const GlobalTracker & copy) = delete;
  ~GlobalTracker() {};
  
  void save_trackable(TrackingInfo * info) {
    tracked_.push_back(info);    
  }  
  
  void report_statistics(std::ostream & output) {
    for (TrackingInfo * info : tracked_) {
      info->report_statistics(output);
    }
  }
  
  void clean_trackables() {
    for (TrackingInfo * info : tracked_) {
      info->reset();
      delete info;
    }
    tracked_.clear();
  }
  
private:
  static GlobalTracker * pointer_;      ///< singleton self pointer
  std::vector<TrackingInfo *> tracked_; ///< array of all tracking info.
};

GlobalTracker * GlobalTracker::pointer_ = nullptr;


/**
 * Singleton object, used to house type based allocation statistics.
 */
template<typename T>
class TypedTrackingInfo : public TrackingInfo{
public:
  static void allocated(TrackingInfo::size_type amount) {
    if (!pointer_) { construct(); }
    pointer_->note_allocation(amount);
  }
  
  static void deallocated(TrackingInfo::size_type amount) {
    if (!pointer_) { construct(); }
    pointer_->note_deallocation(amount);    
  }
  
  virtual void report_statistics(std::ostream & output) {
    output << "Statistics for " << typeid(type).name() << ":\n"
    << "  Total:   " << std::setw(6) << total_ 
    << "  Current: " << std::setw(6) << current_
    << "  Peak:    " << std::setw(6) << peak_
    << std::endl;
  }
  
  virtual void note_allocation(TrackingInfo::size_type amount) {
    current_    += amount;
    total_      += amount;
    peak_        = std::max(peak_, current_);
  }     
  
  virtual void note_deallocation(TrackingInfo::size_type amount) {
    current_    -= amount;
  }
  
  virtual void reset() {
    current_     = 0;
    peak_        = 0;
    total_       = 0;
    pointer_     = nullptr;
  }
protected:
  static void construct() {
    if (pointer_) { return; }
    else {
      //Start thread unsafe code
      pointer_ = new TypedTrackingInfo<T>();
      GlobalTracker::track(pointer_);
      //End thread unsafe code
    }
  }
  
  friend class GlobalTracker;
  
  TypedTrackingInfo()
  : total_(0)
  , current_(0)
  , peak_(0) {};
  TypedTrackingInfo(const TypedTrackingInfo & copy) = delete;
  ~TypedTrackingInfo() {};

private:
  typedef T                        type;
  typename TrackingInfo::size_type total_;      ///< total allocations
  typename TrackingInfo::size_type current_;    ///< current allocations  
  typename TrackingInfo::size_type peak_;       ///< peak allocations  
  
  static TypedTrackingInfo<T> * pointer_;       ///< singleton self pointer
};    //    end of class TypedTrackingInfo

template<typename T>
TypedTrackingInfo<T> * TypedTrackingInfo<T>::pointer_ = nullptr; 

#endif //__type_tracking_hpp__