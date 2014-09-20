#pragma once
#include "system/app.h"
#include "parameter/kv_vector.h"
#include "proto/linear_method.pb.h"

#include "linear_method/loss_inl.h"
#include "linear_method/penalty_inl.h"
// #include "linear_method/learner/learner.h"
// #include "linear_method/learner/aggregate_gradient.h"

namespace PS {
namespace LM {

// linear classification/regerssion
class LinearMethod : public App {
 public:
  virtual void init();

  void process(const MessagePtr& msg);
  void mergeProgress(int iter);
  void mergeAUC(AUC* auc);

 protected:
  void startSystem();

  // load the data, and return 1 if hit cache, 0 if normal, -1 if error,
  virtual int loadData(const MessageCPtr& msg, InstanceInfo* info) = 0;
  virtual void preprocessData(const MessageCPtr& msg) = 0;
  // update model
  virtual void updateModel(const MessagePtr& msg) = 0;
  // compute objective, time, ...
  virtual RiskMinProgress evaluateProgress() = 0;
  virtual void saveModel(const MessageCPtr& msg) = 0;
  virtual void computeEvaluationAUC(AUCData *data) = 0;

  // virtual void saveAsDenseData(const Message& msg) { }

  void showTime(int iter);
  void showObjective(int iter);
  void showNNZ(int iter);

  static Call get(const MessageCPtr& msg) {
    CHECK_EQ(msg->task.type(), Task::CALL_CUSTOMER);
    CHECK(msg->task.has_risk());
    return msg->task.risk();
  }
  static Call* set(Task *task) {
    task->set_type(Task::CALL_CUSTOMER);
    return task->mutable_risk();
  }
  static Task newTask(Call::Command cmd) {
    Task task; set(&task)->set_cmd(cmd);
    return task;
  }

  // progress of all iterations, only valid for the scheduler
  std::map<int, Progress> global_progress_;

  Config conf_;
  Timer timer_;

  LossPtr<double> loss_;
  PenaltyPtr<double> penalty_;
  // shared_ptr<AggGradLearner<double>> learner_;

};

} // namespace LM

} // namespace PS
