# Phase 2: Contribution Transaction System

## Overview
Implement the contribution reward system that allows users to submit contributions (code, charity, creative work, education, mentorship, AI validation, data labeling, ethical review) and receive FC rewards.

## Architecture

### 1. Transaction Type Extension
- Extend `CTransaction` to support contribution transactions
- Add `tx_type` field to distinguish standard vs contribution transactions
- Contribution transactions use `tx_type = 0x01` (per whitepaper)

### 2. Contribution Types (8 Types)
```cpp
enum ContributionType {
    CODE_CONTRIBUTION = 0x01,
    CHARITABLE_ACT = 0x02,
    CREATIVE_WORK = 0x03,
    EDUCATIONAL_CONTENT = 0x04,
    CODE_MENTORSHIP = 0x05,
    AI_VALIDATION = 0x06,
    DATA_LABELING = 0x07,
    ETHICAL_REVIEW = 0x08
};
```

### 3. Contribution Transaction Structure
```cpp
class CContributionTransaction : public CTransaction {
    uint64_t timestamp;
    CPubKey contributor;
    ContributionType contribution_type;
    std::vector<unsigned char> proof_data;
    std::vector<unsigned char> signature;
    
    // For mentorship types
    CPubKey mentor;
    CPubKey mentee;
    std::string feedback;
    uint8_t approval_status; // APPROVED, REJECTED, NEEDS_WORK
    
    // Serialization
    ADD_SERIALIZE_METHODS;
    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        // Base transaction serialization
        READWRITE(nVersion);
        READWRITE(tx_type); // 0x01 for contribution
        
        // Contribution-specific fields
        READWRITE(timestamp);
        READWRITE(contributor);
        READWRITE(contribution_type);
        READWRITE(proof_data);
        READWRITE(signature);
        
        // Optional mentorship fields
        if (contribution_type == CODE_MENTORSHIP) {
            READWRITE(mentor);
            READWRITE(mentee);
            READWRITE(feedback);
            READWRITE(approval_status);
        }
    }
};
```

### 4. Verification System
- **Automated Verification**: GitHub API, GitLab API for code contributions
- **Oracle Verification**: 3/5 majority for charitable acts, creative work, ethical review
- **Human Validator Pools**: Reputation-gated groups for batch verification
- **Quality Checks**: Minimum requirements (lines of code, word count, etc.)

### 5. Block Reward Integration
- Modify `GetFleetCreditsBlockSubsidy` in `fleetcredits.cpp`
- Check for verified contribution transactions in block
- Apply bonus multiplier (1.05x to 1.25x for AI validation)
- Distribute rewards to contributors

### 6. Files to Create/Modify

#### New Files:
- `src/primitives/contribution.h` - Contribution transaction definitions
- `src/primitives/contribution.cpp` - Contribution transaction implementation
- `src/contrib/verification.h` - Verification logic
- `src/contrib/verification.cpp` - Verification implementation
- `src/contrib/oracle.h` - Oracle system
- `src/contrib/oracle.cpp` - Oracle implementation
- `src/rpc/contribution.cpp` - RPC commands for contributions

#### Modified Files:
- `src/primitives/transaction.h` - Add tx_type field
- `src/primitives/transaction.cpp` - Handle contribution transactions
- `src/fleetcredits.cpp` - Integrate contribution rewards into block subsidy
- `src/validation.cpp` - Validate contribution transactions
- `src/Makefile.am` - Add new source files

## Implementation Steps

1. **Week 1**: Transaction structure and serialization
2. **Week 2**: Verification system (automated + oracle stubs)
3. **Week 3**: Block reward integration
4. **Week 4**: RPC commands and testing

## Testing Strategy
- Unit tests for each contribution type
- Integration tests for verification
- Block reward calculation tests
- RPC command tests

