#ifndef __SUBSUMABLE_ACTION_H__
#define __SUBSUMABLE_ACTION_H__

#include <Arduino.h>

template <typename EnvT>
class AbstractSubsumableAction {
 public:
  virtual void Update(const EnvT& env) = 0;
  virtual bool Execute() = 0;
};

template <typename EnvT>
class SubsumptionHierarchy {
 public:
  template <int N>
  SubsumptionHierarchy(AbstractSubsumableAction<EnvT>* (&subsumables)[N]) {
    Init(subsumables, N);
  }

  void UpdateThenExecute(const EnvT& env) {
    for (int i = 0; i < num_sumbsumables_; ++i) {
      sumbsumables_[i]->Update(env);
    }

    for (int i = 0; i < num_sumbsumables_; ++i) {
      if (sumbsumables_[i]->Execute()) {
        break;
      }
    }
  }

 private:
  void Init(AbstractSubsumableAction<EnvT>** actions, int num_actions) {
   sumbsumables_ = actions;
   num_sumbsumables_ = num_actions;
  }

  AbstractSubsumableAction<EnvT>** sumbsumables_;
  int num_sumbsumables_;
};

#endif  // __SUBSUMABLE_ACTION_H__
