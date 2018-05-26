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

  void SetIndexPIDStatus(int i, int p, int t);
  inline int Index() const { return j_index; }
  inline int PID() const { return j_PID; }
  inline int Status() const { return j_status; }
  void SetMother(int mpid, int mindex);
  inline int MotherPID() const { return j_mother_PID; }
  inline int MotherIndex() const { return j_mother_index; }

  //==== Gen Status Flags
  inline void SetGenStatusFlag_isPrompt(bool b){ j_isPrompt = b; }
  inline void SetGenStatusFlag_isPromptFinalState(bool b){ j_isPromptFinalState = b; }
  inline void SetGenStatusFlag_isTauDecayProduct(bool b){ j_isTauDecayProduct = b; }
  inline void SetGenStatusFlag_isPromptTauDecayProduct(bool b){ j_isPromptTauDecayProduct = b; }
  inline void SetGenStatusFlag_isDirectPromptTauDecayProductFinalState(bool b){ j_isDirectPromptTauDecayProductFinalState = b; }
  inline void SetGenStatusFlag_isHardProcess(bool b){ j_isHardProcess = b; }
  inline void SetGenStatusFlag_isLastCopy(bool b){ j_isLastCopy = b; }
  inline void SetGenStatusFlag_isLastCopyBeforeFSR(bool b){ j_isLastCopyBeforeFSR = b; }
  inline void SetGenStatusFlag_isPromptDecayed(bool b){ j_isPromptDecayed = b; }
  inline void SetGenStatusFlag_isDecayedLeptonHadron(bool b){ j_isDecayedLeptonHadron = b; }
  inline void SetGenStatusFlag_fromHardProcessBeforeFSR(bool b){ j_fromHardProcessBeforeFSR = b; }
  inline void SetGenStatusFlag_fromHardProcessDecayed(bool b){ j_fromHardProcessDecayed = b; }
  inline void SetGenStatusFlag_fromHardProcessFinalState(bool b){ j_fromHardProcessFinalState = b; }
  inline void SetGenStatusFlag_isMostlyLikePythia6Status3(bool b){ j_isMostlyLikePythia6Status3 = b; }

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
  int j_index, j_PID, j_status;
  int j_mother_PID, j_mother_index;
  bool j_isPrompt, j_isPromptFinalState, j_isTauDecayProduct, j_isPromptTauDecayProduct, j_isDirectPromptTauDecayProductFinalState, j_isHardProcess, j_isLastCopy, j_isLastCopyBeforeFSR, j_isPromptDecayed, j_isDecayedLeptonHadron, j_fromHardProcessBeforeFSR, j_fromHardProcessDecayed, j_fromHardProcessFinalState, j_isMostlyLikePythia6Status3;

};

Gen::Gen() : Particle() {
  j_IsEmpty = true;
  j_index = -1;
  j_PID = 0;
  j_status = 0;
  j_mother_PID = 0;
  j_mother_index = -1;
  j_isPrompt = false;
  j_isPromptFinalState = false;
  j_isTauDecayProduct = false;
  j_isPromptTauDecayProduct = false;
  j_isDirectPromptTauDecayProductFinalState = false;
  j_isHardProcess = false;
  j_isLastCopy = false;
  j_isLastCopyBeforeFSR = false;
  j_isPromptDecayed = false;
  j_isDecayedLeptonHadron = false;
  j_fromHardProcessBeforeFSR = false;
  j_fromHardProcessDecayed = false;
  j_fromHardProcessFinalState = false;
  j_isMostlyLikePythia6Status3 = false;
}

Gen::~Gen(){
}

void Gen::SetIndexPIDStatus(int i, int p, int t){
  j_index = i;
  j_PID = p;
  j_status = t;
}

void Gen::SetMother(int mpid, int mindex){
  j_mother_PID = mpid;
  j_mother_index = mindex;
}

#endif
