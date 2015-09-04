#ifndef __l1t_gmt_internal_muon_h__
#define __l1t_gmt_internal_muon_h__

#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/L1TMuon/interface/L1TRegionalMuonCandidate.h"
#include "DataFormats/L1TMuon/interface/L1TRegionalMuonCandidateFwd.h"
#include <utility>

namespace l1t {
class L1TGMTInternalMuon {
  public:
    explicit L1TGMTInternalMuon(const edm::Handle<L1TRegionalMuonCandidateCollection>&, size_t);
    L1TGMTInternalMuon(const L1TGMTInternalMuon&);
    L1TGMTInternalMuon() {};

    virtual ~L1TGMTInternalMuon() {};

    void setHwCancelBit(int bit) { m_hwCancelBit = bit; };
    void setHwRank(int bits) { m_hwRank = bits; };
    void setHwWins(int wins) { m_hwWins = wins; };
    void increaseWins() { m_hwWins++; };
    void setHwIsoSum(int isosum) { m_hwIsoSum = isosum; };
    void setHwAbsIso(int iso) { m_hwAbsIso = iso; };
    void setHwRelIso(int iso) { m_hwRelIso = iso; };
    void setExtrapolation(int deta, int dphi);
    void setHwCaloEta(int idx) { m_hwCaloIndex.second = idx; };
    void setHwCaloPhi(int idx) { m_hwCaloIndex.first = idx; };
    void setTFType(tftype type) { m_realtype = type; };

    static int calcGlobalPhi(int locPhi, tftype t, int proc);

    const int hwCancelBit() const { return m_hwCancelBit; };
    const int hwRank() const { return m_hwRank; };
    const int hwWins() const { return m_hwWins; };
    const int hwIsoSum() const { return m_hwIsoSum; };
    const int hwDEta() const { return m_hwDeltaEta; };
    const int hwDPhi() const { return m_hwDeltaPhi; };
    const int hwAbsIso() const { return m_hwAbsIso; };
    const int hwRelIso() const { return m_hwRelIso; };
    const int hwCaloEta() const { return m_hwCaloIndex.second; };
    const int hwCaloPhi() const { return m_hwCaloIndex.first; };
    const int hwGlobalPhi() const { return m_hwGlobalPhi; }


    const L1TRegionalMuonCandidate& origin() const { return *m_regional; };
    const edm::Ref<L1TRegionalMuonCandidateCollection> originRef() const { return m_regional; };

    inline const int hwPt() const { return m_regional->hwPt(); };
    inline const int hwLocalPhi() const { return m_regional->hwPhi(); };
    inline const int hwEta() const { return m_regional->hwEta(); };
    inline const int hwSign() const { return m_regional->hwSign(); };
    inline const int hwSignValid() const { return m_regional->hwSignValid(); };
    inline const int hwQual() const { return m_regional->hwQual(); };
    inline const int hwTrackAddress() const { return m_regional->hwTrackAddress(); };
    inline const int processor() const { return m_regional->processor(); };
    inline const tftype trackFinderType() const { return m_realtype; };
    inline const int link() const { return m_regional->link(); }

  private:

    const edm::Ref<L1TRegionalMuonCandidateCollection> m_regional;
    int m_hwRank;
    int m_hwCancelBit;
    int m_hwWins;
    int m_hwIsoSum;
    int m_hwDeltaEta;
    int m_hwDeltaPhi;
    int m_hwAbsIso;
    int m_hwRelIso;
    int m_hwGlobalPhi;
    tftype m_realtype;
    std::pair<int, int> m_hwCaloIndex;
};

} // namespace l1t

#endif /* define __l1t_gmt_internal_muon_h__ */
