#ifndef Gen_h
#define Gen_h

#include "Particle.h"

class Gen: public Particle {
public:

  Gen();
  ~Gen();

  //==== For empty Gen
  inline void SetIsEmpty(bool b) { j_IsEmpty = b; }
  inline bool IsEmpty() const { return j_IsEmpty; }

  void SetPIDandStatus(int p, int t);
  inline int PID() const { return j_PID; }
  inline int Status() const { return j_status; }
  void SetMother(int mpid, int mindex);
  inline int MotherPID() const { return j_mother_PID; }
  inline int MotherIndex() const { return j_mother_index; }

  //==== Gen Status Flags
  inline void SetGenStatusFlag_isPrompt(int i){ j_isPrompt = i; }
  inline void SetGenStatusFlag_isPromptFinalState(int i){ j_isPromptFinalState = i; }
  inline void SetGenStatusFlag_isTauDecayProduct(int i){ j_isTauDecayProduct = i; }
  inline void SetGenStatusFlag_isPromptTauDecayProduct(int i){ j_isPromptTauDecayProduct = i; }
  inline void SetGenStatusFlag_isDirectPromptTauDecayProductFinalState(int i){ j_isDirectPromptTauDecayProductFinalState = i; }
  inline void SetGenStatusFlag_isHardProcess(int i){ j_isHardProcess = i; }
  inline void SetGenStatusFlag_isLastCopy(int i){ j_isLastCopy = i; }
  inline void SetGenStatusFlag_isLastCopyBeforeFSR(int i){ j_isLastCopyBeforeFSR = i; }
  inline void SetGenStatusFlag_isPromptDecayed(int i){ j_isPromptDecayed = i; }
  inline void SetGenStatusFlag_isDecayedLeptonHadron(int i){ j_isDecayedLeptonHadron = i; }
  inline void SetGenStatusFlag_fromHardProcessBeforeFSR(int i){ j_fromHardProcessBeforeFSR = i; }
  inline void SetGenStatusFlag_fromHardProcessDecayed(int i){ j_fromHardProcessDecayed = i; }
  inline void SetGenStatusFlag_fromHardProcessFinalState(int i){ j_fromHardProcessFinalState = i; }
  inline void SetGenStatusFlag_isMostlyLikePythia6Status3(int i){ j_isMostlyLikePythia6Status3 = i; }

  inline bool isPrompt() const { return j_isPrompt; }
  inline bool isPromptFinalState() const { return j_isPromptFinalState; }
  inline bool isTauDecayProduct() const { return j_isTauDecayProduct; }
  inline bool isPromptTauDecayProduct() const { return j_isPromptTauDecayProduct; }
  inline bool isDirectPromptTauDecayProductFinalState() const { return j_isDirectPromptTauDecayProductFinalState; }
  inline bool isHardProcess() const { return j_isHardProcess; }
  inline bool isLastCopy() const { return j_isLastCopy; }
  inline bool isLastCopyBeforeFSR() const { return j_isLastCopyBeforeFSR; }
  inline bool isPromptDecayed() const { return j_isPromptDecayed; }
  inline bool isDecayedLeptonHadron() const { return j_isDecayedLeptonHadron; }
  inline bool fromHardProcessBeforeFSR() const { return j_fromHardProcessBeforeFSR; }
  inline bool fromHardProcessDecayed() const { return j_fromHardProcessDecayed; }
  inline bool fromHardProcessFinalState() const { return j_fromHardProcessFinalState; }
  inline bool isMostlyLikePythia6Status3() const { return j_isMostlyLikePythia6Status3; }

private:

  bool j_IsEmpty;
  int j_PID, j_status;
  int j_mother_PID, j_mother_index;
  int j_isPrompt, j_isPromptFinalState, j_isTauDecayProduct, j_isPromptTauDecayProduct, j_isDirectPromptTauDecayProductFinalState, j_isHardProcess, j_isLastCopy, j_isLastCopyBeforeFSR, j_isPromptDecayed, j_isDecayedLeptonHadron, j_fromHardProcessBeforeFSR, j_fromHardProcessDecayed, j_fromHardProcessFinalState, j_isMostlyLikePythia6Status3;

};

Gen::Gen() : Particle() {
  j_IsEmpty = true;
}

Gen::~Gen(){
}

void Gen::SetPIDandStatus(int p, int t){
  j_PID = p;
  j_status = t;
}

void Gen::SetMother(int mpid, int mindex){
  j_mother_PID = mpid;
  j_mother_index = mindex;
}

#endif
