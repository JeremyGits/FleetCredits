# Fleet Credits Global Entity-Relationship Diagram
## Complete System Architecture (Post-Conversion)

This ERD represents the entire Fleet Credits blockchain ecosystem as implemented in the codebase, from core blockchain operations to MWEB privacy layers, contribution verification, reward distribution, governance, and network infrastructure.

**Last Updated**: Post-complete ERD expansion with ALL entity attributes
**Version**: 4.0 - Complete Comprehensive Blueprint
**Status**: ✅ ALL entities now have FULL attribute definitions (no "id" only entities)

```mermaid
erDiagram
    %% ==========================================
    %% CORE BLOCKCHAIN LAYER
    %% ==========================================
    CBLOCK ||--o{ CTRANSACTION : "contains"
    CBLOCK ||--o| CBLOCKHEADER : "has"
    CBLOCK ||--o| CMWEBEXTENSIONBLOCK : "pegs to"
    CBLOCK ||--|| BLOCK_SUBSIDY : "generates"
    CBLOCK ||--o| BLOCK_REWARD : "distributes"
    CBLOCKHEADER ||--o| CPUREBLOCKHEADER : "extends"
    CBLOCKHEADER ||--o| CAUXPOW : "may have"
    CBLOCKHEADER ||--|| MERKLE_ROOT : "includes"
    MERKLE_ROOT ||--o{ CTRANSACTION : "hashes"
    
    CTRANSACTION ||--o{ CTXIN : "has"
    CTRANSACTION ||--o{ CTXOUT : "creates"
    CTRANSACTION ||--o| CCONTRIBUTIONTRANSACTION : "embeds"
    CTRANSACTION ||--o| OP_RETURN_DATA : "may contain"
    
    CTXIN ||--|| COUTPOINT : "references"
    CTXIN ||--|| CScript : "contains scriptSig"
    CTXOUT ||--|| CScript : "contains scriptPubKey"
    CTXOUT ||--|| CAmount : "has value"
    
    COUTPOINT ||--|| UTXO : "spends"
    CTXOUT ||--|| UTXO : "creates"
    UTXO ||--|| CFleetCreditsAddress : "belongs to"
    
    %% ==========================================
    %% BLOCK SUBSIDY & REWARD SYSTEM
    %% ==========================================
    BLOCK_SUBSIDY ||--|| BASE_REWARD : "fixed at 10000 FC"
    BLOCK_SUBSIDY ||--o| BONUS_MULTIPLIER : "applies from contributions"
    BLOCK_REWARD ||--|| BLOCK_SUBSIDY : "calculated from"
    BLOCK_REWARD ||--|| COINBASE_TX : "added to"
    BLOCK_REWARD ||--o{ REWARD_PAYOUT : "distributes to contributors"
    
    COINBASE_TX ||--|| CTRANSACTION : "is first transaction"
    COINBASE_TX ||--|| CAmount : "contains block reward"
    COINBASE_TX ||--o{ CONTRIBUTOR_ADDRESS : "sends to contributor"
    
    BONUS_MULTIPLIER ||--|| BONUS_LEVEL : "determined by"
    BONUS_LEVEL ||--|| BONUS_NONE : "1.0x"
    BONUS_LEVEL ||--|| BONUS_LOW : "1.05x"
    BONUS_LEVEL ||--|| BONUS_MEDIUM : "1.10x"
    BONUS_LEVEL ||--|| BONUS_HIGH : "1.15x"
    BONUS_LEVEL ||--|| BONUS_CRITICAL : "1.20x (1.25x for AI_VALIDATION)"
    
    %% ==========================================
    %% CONTRIBUTION TRANSACTION SYSTEM
    %% ==========================================
    CCONTRIBUTIONTRANSACTION ||--|| CPROOFDATA : "embeds"
    CCONTRIBUTIONTRANSACTION ||--|| CONTRIBUTIONTYPE : "categorized as"
    CCONTRIBUTIONTRANSACTION ||--|| CPubKey : "submitted by (contributor)"
    CCONTRIBUTIONTRANSACTION ||--|| CFleetCreditsAddress : "reward sent to"
    CCONTRIBUTIONTRANSACTION ||--o| CMENTORSHIPTRANSACTION : "extends to"
    CCONTRIBUTIONTRANSACTION ||--o| CAIVALIDATIONTRANSACTION : "extends to"
    CCONTRIBUTIONTRANSACTION ||--o| CDATALABELINGTRANSACTION : "extends to"
    CCONTRIBUTIONTRANSACTION ||--o| CETHICALREVIEWTRANSACTION : "extends to"
    CCONTRIBUTIONTRANSACTION ||--o| CCODECONTRIBUTIONTRANSACTION : "extends to"
    CCONTRIBUTIONTRANSACTION ||--o| CCREATIVEWORKTRANSACTION : "extends to"
    CCONTRIBUTIONTRANSACTION ||--o| CEDUCATIONALCONTENTTRANSACTION : "extends to"
    CCONTRIBUTIONTRANSACTION ||--o| CCHARITABLEACTTRANSACTION : "extends to"
    CCONTRIBUTIONTRANSACTION ||--|| CVERIFICATIONRECORD : "triggers"
    CCONTRIBUTIONTRANSACTION ||--o| REWARD_PAYOUT : "earns"
    CCONTRIBUTIONTRANSACTION ||--o| CHALLENGE_TX : "challenged by"
    CCONTRIBUTIONTRANSACTION ||--o| CMWEBTRANSACTION : "blinded variant"
    
    CONTRIBUTIONTYPE ||--|| CODE_CONTRIBUTION : "0x01"
    CONTRIBUTIONTYPE ||--|| CHARITABLE_ACT : "0x02"
    CONTRIBUTIONTYPE ||--|| CREATIVE_WORK : "0x03"
    CONTRIBUTIONTYPE ||--|| EDUCATIONAL_CONTENT : "0x04"
    CONTRIBUTIONTYPE ||--|| CODE_MENTORSHIP : "0x05"
    CONTRIBUTIONTYPE ||--|| AI_VALIDATION : "0x06"
    CONTRIBUTIONTYPE ||--|| DATA_LABELING : "0x07"
    CONTRIBUTIONTYPE ||--|| ETHICAL_REVIEW : "0x08"
    
    CPROOFDATA ||--|| uint256 : "has hash"
    CPROOFDATA ||--|| CONTRIBUTIONTYPE : "has contrib_type"
    CPROOFDATA ||--o{ evidence : "vector<unsigned char>"
    CPROOFDATA ||--o{ nonce : "vector<unsigned char>"
    CPROOFDATA ||--|| uint64_t : "has timestamp"
    CPROOFDATA ||--o{ metadata : "vector<unsigned char>"
    
    CPROOFDATA ||--o| GITHUB_COMMIT : "references"
    CPROOFDATA ||--o| IPFS_HASH : "stores"
    CPROOFDATA ||--o| AI_OUTPUT_HASH : "validates"
    CPROOFDATA ||--o| DATASET_LABEL : "contains"
    CPROOFDATA ||--o| ETHICAL_RATIONALE : "includes"
    
    CMENTORSHIPTRANSACTION ||--|| CPubKey : "has mentor"
    CMENTORSHIPTRANSACTION ||--|| CPubKey : "has mentee"
    CMENTORSHIPTRANSACTION ||--|| APPROVALSTATUS : "has approval_status"
    CMENTORSHIPTRANSACTION ||--|| uint256 : "has feedback_hash"
    CMENTORSHIPTRANSACTION ||--|| uint256 : "has improvement_commit_hash"
    
    APPROVALSTATUS ||--|| APPROVED : "0x01"
    APPROVALSTATUS ||--|| REJECTED : "0x02"
    APPROVALSTATUS ||--|| NEEDS_WORK : "0x03"
    
    CAIVALIDATIONTRANSACTION ||--|| string : "has ai_model_id"
    CAIVALIDATIONTRANSACTION ||--|| float : "has accuracy_score >= 0.8"
    CAIVALIDATIONTRANSACTION ||--|| uint256 : "has validation_report_hash"
    CAIVALIDATIONTRANSACTION ||--|| uint256 : "has human_override_hash"
    CAIVALIDATIONTRANSACTION ||--|| bool : "has human_override_verified"
    
    CDATALABELINGTRANSACTION ||--|| string : "has dataset_id"
    CDATALABELINGTRANSACTION ||--|| uint32_t : "has labels_count"
    CDATALABELINGTRANSACTION ||--|| uint256 : "has labels_hash"
    CDATALABELINGTRANSACTION ||--|| float : "has inter_annotator_agreement >= 0.7"
    CDATALABELINGTRANSACTION ||--|| uint32_t : "has annotator_count >= 3"
    
    CETHICALREVIEWTRANSACTION ||--|| bool : "requires_mweb = true"
    CETHICALREVIEWTRANSACTION ||--|| string : "has ethical_rationale"
    CETHICALREVIEWTRANSACTION ||--|| uint256 : "has review_report_hash"
    
    %% ==========================================
    %% MWEB (MIMBLEWIMBLE EXTENSION BLOCKS)
    %% ==========================================
    CMWEBEXTENSIONBLOCK ||--o{ CMWEBTRANSACTION : "contains"
    CMWEBEXTENSIONBLOCK ||--o{ CPEGINTRANSACTION : "includes"
    CMWEBEXTENSIONBLOCK ||--o{ CPEGOUTTRANSACTION : "includes"
    
    CMWEBTRANSACTION ||--o{ CMWEBINPUT : "has"
    CMWEBTRANSACTION ||--o{ CMWEBOUTPUT : "has"
    CMWEBTRANSACTION ||--|| CMWEBKERNEL : "has"
    CMWEBTRANSACTION ||--|| CCONTRIBUTIONTRANSACTION : "may embed"
    
    CMWEBINPUT ||--|| CPEDERSENCOMMITMENT : "references output_commitment"
    CMWEBINPUT ||--o{ witness : "vector<unsigned char>"
    
    CMWEBOUTPUT ||--|| CPEDERSENCOMMITMENT : "has commitment"
    CMWEBOUTPUT ||--|| CRANGEPROOF : "has range_proof"
    CMWEBOUTPUT ||--o{ view_key : "vector<unsigned char>"
    
    CPEDERSENCOMMITMENT ||--|| CPubKey : "commitment = vG + rH"
    CPEDERSENCOMMITMENT ||--|| uint256 : "has blinding_factor_hash"
    
    CRANGEPROOF ||--o{ proof_data : "vector<unsigned char>"
    CRANGEPROOF ||--|| uint32_t : "value_range_min"
    CRANGEPROOF ||--|| uint32_t : "value_range_max"
    
    CMWEBKERNEL ||--|| CPubKey : "has excess_value"
    CMWEBKERNEL ||--o{ signature : "vector<unsigned char>"
    CMWEBKERNEL ||--|| uint64_t : "has fee"
    CMWEBKERNEL ||--|| uint256 : "has kernel_id"
    
    CPEGINTRANSACTION ||--|| uint256 : "has peg_tx_id"
    CPEGINTRANSACTION ||--|| uint256 : "has main_chain_tx_id"
    CPEGINTRANSACTION ||--|| CAmount : "has amount"
    CPEGINTRANSACTION ||--|| CPubKey : "has mweb_address"
    
    CPEGOUTTRANSACTION ||--|| uint256 : "has peg_tx_id"
    CPEGOUTTRANSACTION ||--|| CPubKey : "has commitment"
    CPEGOUTTRANSACTION ||--|| CAmount : "has amount"
    CPEGOUTTRANSACTION ||--|| CFleetCreditsAddress : "has main_chain_address"
    
    CVIEWKEY ||--|| uint256 : "has seed"
    CVIEWKEY ||--|| CPubKey : "has view_pubkey"
    CVIEWKEY ||--|| CPubKey : "has scan_pubkey"
    
    %% ==========================================
    %% VERIFICATION & CONSENSUS SYSTEM
    %% ==========================================
    CVERIFICATIONRECORD ||--|| uint256 : "has record_id"
    CVERIFICATIONRECORD ||--|| uint256 : "has contrib_tx_id"
    CVERIFICATIONRECORD ||--o{ CORACLEVOTE : "receives"
    CVERIFICATIONRECORD ||--|| float : "has consensus_score"
    CVERIFICATIONRECORD ||--|| bool : "has approved"
    CVERIFICATIONRECORD ||--|| VERIFICATIONSTATUS : "has verification_status"
    CVERIFICATIONRECORD ||--|| uint32_t : "has oracle_vote_count"
    CVERIFICATIONRECORD ||--|| uint32_t : "has required_votes (typically 3)"
    CVERIFICATIONRECORD ||--o{ HUMANVALIDATORVOTE : "includes"
    CVERIFICATIONRECORD ||--o| CVIEWKEY : "uses for MWEB"
    CVERIFICATIONRECORD ||--o| ETHICAL_EVALUATION : "subject to"
    
    ETHICAL_EVALUATION ||--|| CONTRIBUTIONTYPE : "evaluates"
    ETHICAL_EVALUATION ||--|| ETHICAL_CRITERIA : "uses"
    ETHICAL_EVALUATION ||--o{ CORACLEVOTE : "reflected in"
    
    ETHICAL_CRITERIA ||--|| OBJECTIVE_CRITERIA : "has"
    ETHICAL_CRITERIA ||--|| VALUE_JUDGMENT_FRAMEWORK : "includes"
    
    OBJECTIVE_CRITERIA ||--|| VALID_PROOF : "requires"
    OBJECTIVE_CRITERIA ||--|| NON_DUPLICATE : "requires"
    OBJECTIVE_CRITERIA ||--|| MINIMUM_THRESHOLDS : "requires"
    
    VALUE_JUDGMENT_FRAMEWORK ||--|| IMPACT_ASSESSMENT : "measures"
    VALUE_JUDGMENT_FRAMEWORK ||--|| AUTHENTICITY_CHECK : "verifies"
    VALUE_JUDGMENT_FRAMEWORK ||--|| ALIGNMENT_WITH_PUBLIC_GOOD : "evaluates"
    VALUE_JUDGMENT_FRAMEWORK ||--|| HARM_PREVENTION : "ensures"
    
    VERIFICATIONSTATUS ||--|| VERIFICATION_PENDING : "0x01"
    VERIFICATIONSTATUS ||--|| VERIFICATION_APPROVED : "0x02"
    VERIFICATIONSTATUS ||--|| VERIFICATION_REJECTED : "0x03"
    VERIFICATIONSTATUS ||--|| VERIFICATION_CHALLENGED : "0x04"
    
    CORACLENODE ||--|| CPubKey : "has pubkey"
    CORACLENODE ||--|| string : "has node_address"
    CORACLENODE ||--|| int64_t : "has stake_fc >= 500k"
    CORACLENODE ||--|| int32_t : "has reputation_score"
    CORACLENODE ||--|| uint32_t : "has total_votes_cast"
    CORACLENODE ||--|| uint32_t : "has correct_votes"
    CORACLENODE ||--|| uint32_t : "has incorrect_votes"
    CORACLENODE ||--|| bool : "has is_active"
    CORACLENODE ||--|| uint64_t : "has last_rotation_date"
    CORACLENODE ||--o{ CORACLEVOTE : "casts"
    CORACLENODE ||--o{ SLASHING_RECORD : "subject to"
    CORACLENODE ||--o{ VALIDATORPOOL : "member of"
    
    CORACLEVOTE ||--|| uint256 : "has vote_id"
    CORACLEVOTE ||--|| CPubKey : "has oracle_pubkey"
    CORACLEVOTE ||--|| uint256 : "has verification_record_id"
    CORACLEVOTE ||--|| ORACLEVOTECHOICE : "has vote_choice"
    CORACLEVOTE ||--|| uint64_t : "has vote_timestamp"
    CORACLEVOTE ||--o{ vote_signature : "vector<unsigned char>"
    CORACLEVOTE ||--|| string : "has vote_reason"
    
    ORACLEVOTECHOICE ||--|| VOTE_APPROVE : "0x01"
    ORACLEVOTECHOICE ||--|| VOTE_REJECT : "0x02"
    ORACLEVOTECHOICE ||--|| VOTE_ABSTAIN : "0x03"
    
    VALIDATORPOOL ||--|| uint256 : "has pool_id"
    VALIDATORPOOL ||--|| int32_t : "has min_reputation"
    VALIDATORPOOL ||--|| uint32_t : "has member_count"
    VALIDATORPOOL ||--|| int64_t : "has total_stake"
    VALIDATORPOOL ||--|| float : "has consensus_score"
    VALIDATORPOOL ||--|| bool : "has is_active"
    VALIDATORPOOL ||--o{ HUMANVALIDATOR : "includes"
    VALIDATORPOOL ||--o{ BATCH_VERIFICATION : "performs"
    
    HUMANVALIDATOR ||--|| uint256 : "has validator_id"
    HUMANVALIDATOR ||--|| CFleetCreditsAddress : "has address"
    HUMANVALIDATOR ||--|| int32_t : "has reputation"
    HUMANVALIDATOR ||--|| uint32_t : "has verified_contributions"
    HUMANVALIDATOR ||--|| int64_t : "has stake"
    HUMANVALIDATOR ||--o{ HUMANVALIDATORVOTE : "casts"
    HUMANVALIDATOR ||--o{ INTER_ANNOTATOR_AGREEMENT : "participates in"
    
    HUMANVALIDATORVOTE ||--|| uint256 : "has vote_id"
    HUMANVALIDATORVOTE ||--|| CPubKey : "has validator_pubkey"
    HUMANVALIDATORVOTE ||--|| uint256 : "has verification_record_id"
    HUMANVALIDATORVOTE ||--|| bool : "has approval"
    HUMANVALIDATORVOTE ||--|| uint64_t : "has vote_timestamp"
    
    INTER_ANNOTATOR_AGREEMENT ||--|| uint256 : "has agreement_id"
    INTER_ANNOTATOR_AGREEMENT ||--|| float : "has agreement_score >= 0.7"
    INTER_ANNOTATOR_AGREEMENT ||--|| uint32_t : "has annotator_count >= 3"
    INTER_ANNOTATOR_AGREEMENT ||--o{ ANNOTATOR_VOTE : "includes"
    
    SLASHING_RECORD ||--|| uint256 : "has slashing_id"
    SLASHING_RECORD ||--|| CPubKey : "has oracle_pubkey"
    SLASHING_RECORD ||--|| uint256 : "has bad_vote_id"
    SLASHING_RECORD ||--|| CAmount : "has slashed_amount (50% of stake)"
    SLASHING_RECORD ||--|| uint64_t : "has slashing_timestamp"
    
    %% ==========================================
    %% ADDRESS & WALLET SYSTEM
    %% ==========================================
    CFleetCreditsAddress ||--o{ CWALLET : "manages"
    CFleetCreditsAddress ||--o{ REPUTATION_SCORE : "has"
    CFleetCreditsAddress ||--o{ VERIFIED_IDENTITY : "linked to"
    CFleetCreditsAddress ||--o{ STAKING_RECORD : "has"
    CFleetCreditsAddress ||--o{ CCONTRIBUTIONTRANSACTION : "submits"
    CFleetCreditsAddress ||--o{ VOTE_TX : "casts"
    CFleetCreditsAddress ||--o{ CHALLENGE_TX : "flags"
    CFleetCreditsAddress ||--o{ REWARD_PAYOUT : "receives"
    
    CWALLET ||--o{ CKey : "manages"
    CWALLET ||--o{ CPubKey : "generates"
    CWALLET ||--o{ CTXOUT : "tracks outputs"
    CWALLET ||--o{ CTRANSACTION : "tracks transactions"
    CWALLET ||--o{ MWEB_WALLET : "includes"
    CWALLET ||--|| CWALLETDB : "persists to"
    
    CWALLETDB ||--|| CWallet : "stores"
    CWALLETDB ||--o{ CKey : "stores keys"
    CWALLETDB ||--o{ CTXOUT : "stores outputs"
    
    MWEB_WALLET ||--|| CWALLET : "belongs to"
    MWEB_WALLET ||--o{ CMWEBOUTPUT : "tracks MWEB UTXOs"
    MWEB_WALLET ||--|| CVIEWKEY : "has view_key"
    
    CFleetCreditsSecret ||--|| CKey : "wraps"
    CFleetCreditsExtKey ||--|| CExtKey : "extends"
    CFleetCreditsExtPubKey ||--|| CExtPubKey : "extends"
    
    %% ==========================================
    %% FEE STRUCTURE SYSTEM
    %% ==========================================
    CFeeRate ||--|| CAmount : "has fee_per_kb"
    CFeeRate ||--|| MICRO_TX_THRESHOLD : "checks against"
    
    MICRO_TX_THRESHOLD ||--|| CAmount : "1000 * COIN"
    
    TRANSACTION_FEE ||--|| CFeeRate : "calculated from"
    TRANSACTION_FEE ||--o| ZERO_FEE : "if amount < MICRO_TX_THRESHOLD"
    TRANSACTION_FEE ||--o| SCALABLE_FEE : "if amount >= MICRO_TX_THRESHOLD"
    
    COMMUNITY_RESERVE ||--|| TRANSACTION_FEE : "receives 1%"
    COMMUNITY_RESERVE ||--|| MULTISIG_WALLET : "managed by"
    
    %% ==========================================
    %% CHAIN PARAMETERS & NETWORK
    %% ==========================================
    CCHAINPARAMS ||--|| CONSENSUS_PARAMS : "has"
    CCHAINPARAMS ||--|| CBLOCK : "has genesis"
    CCHAINPARAMS ||--|| CMessageHeader : "has pchMessageStart"
    CCHAINPARAMS ||--|| int : "has nDefaultPort"
    CCHAINPARAMS ||--|| CDNSSeedData : "has vSeeds"
    CCHAINPARAMS ||--|| CCheckpointData : "has checkpointData"
    CCHAINPARAMS ||--|| ChainTxData : "has chainTxData"
    
    CONSENSUS_PARAMS ||--|| CAmount : "nSubsidy = 10000 * COIN"
    CONSENSUS_PARAMS ||--|| int64_t : "nTargetTimespan = 60 seconds"
    CONSENSUS_PARAMS ||--|| int64_t : "nTargetSpacing = 60 seconds"
    CONSENSUS_PARAMS ||--|| uint256 : "nMinimumChainWork"
    CONSENSUS_PARAMS ||--|| uint256 : "defaultAssumeValid"
    CONSENSUS_PARAMS ||--|| uint256 : "hashGenesisBlock"
    
    CCHAINPARAMS ||--|| MAINNET : "port 22556/22555"
    CCHAINPARAMS ||--|| TESTNET : "port 44556/44555"
    CCHAINPARAMS ||--|| REGTEST : "port 18444/18332"
    
    CMessageHeader ||--|| uint8_t : "pchMessageStart[0] = 0xfc"
    CMessageHeader ||--|| uint8_t : "pchMessageStart[1] = 0xc1"
    CMessageHeader ||--|| uint8_t : "pchMessageStart[2] = 0xb7"
    CMessageHeader ||--|| uint8_t : "pchMessageStart[3] = 0xdc"
    
    %% ==========================================
    %% GENESIS BLOCK MINING & VALIDATION
    %% ==========================================
    GENESIS_BLOCK_MINING ||--|| CBLOCK : "mines genesis"
    GENESIS_BLOCK_MINING ||--|| CreateGenesisBlock : "creates block"
    GENESIS_BLOCK_MINING ||--|| CheckProofOfWork : "validates PoW"
    GENESIS_BLOCK_MINING ||--|| GetPoWHash : "calculates Scrypt hash"
    GENESIS_BLOCK_MINING ||--|| uint32_t : "increments nNonce"
    GENESIS_BLOCK_MINING ||--|| CONSENSUS_PARAMS : "uses params"
    
    GENESIS_BLOCK_VALIDATION ||--|| CBLOCK : "validates genesis"
    GENESIS_BLOCK_VALIDATION ||--|| CheckAuxPowProofOfWork : "validates PoW"
    GENESIS_BLOCK_VALIDATION ||--|| CONSENSUS_PARAMS : "checks hashGenesisBlock"
    GENESIS_BLOCK_VALIDATION ||--|| CCheckpointData : "validates checkpoint"
    
    CCheckpointData ||--|| uint256 : "has genesis hash at height 0"
    CCheckpointData ||--|| CBLOCK : "references genesis block"
    
    %% ==========================================
    %% BUILD SYSTEM & LIBRARIES
    %% ==========================================
    BUILD_SYSTEM ||--|| LIBFLEETCREDITS_SERVER : "libfleetcredits_server.a"
    BUILD_SYSTEM ||--|| LIBFLEETCREDITS_COMMON : "libfleetcredits_common.a"
    BUILD_SYSTEM ||--|| LIBFLEETCREDITS_CONSENSUS : "libfleetcredits_consensus.a"
    BUILD_SYSTEM ||--|| LIBFLEETCREDITS_CLI : "libfleetcredits_cli.a"
    BUILD_SYSTEM ||--|| LIBFLEETCREDITS_UTIL : "libfleetcredits_util.a"
    BUILD_SYSTEM ||--|| LIBFLEETCREDITS_CRYPTO : "crypto/libfleetcredits_crypto.a"
    BUILD_SYSTEM ||--|| LIBFLEETCREDITS_QT : "qt/libfleetcreditsqt.a"
    BUILD_SYSTEM ||--|| LIBFLEETCREDITS_WALLET : "libfleetcredits_wallet.a"
    BUILD_SYSTEM ||--|| LIBFLEETCREDITS_ZMQ : "libfleetcredits_zmq.a"
    BUILD_SYSTEM ||--|| LIBFLEETCREDITSCONSENSUS : "libfleetcreditsconsensus.la"
    
    BUILD_SYSTEM ||--|| FLEETCREDITS_DAEMON : "fleetcreditsd"
    BUILD_SYSTEM ||--|| FLEETCREDITS_QT : "fleetcredits-qt"
    BUILD_SYSTEM ||--|| FLEETCREDITS_CLI : "fleetcredits-cli"
    BUILD_SYSTEM ||--|| FLEETCREDITS_TX : "fleetcredits-tx"
    
    LIBFLEETCREDITS_SERVER ||--o{ fleetcreditsd : "linked to"
    LIBFLEETCREDITS_SERVER ||--o{ fleetcredits_qt : "linked to"
    LIBFLEETCREDITS_COMMON ||--o{ all_executables : "linked to"
    LIBFLEETCREDITS_CONSENSUS ||--o{ fleetcredits_qt : "linked to"
    LIBFLEETCREDITS_CONSENSUS ||--o{ fleetcreditsd : "linked to"
    LIBFLEETCREDITS_CLI ||--o{ fleetcredits_cli : "linked to"
    LIBFLEETCREDITS_UTIL ||--o{ all_executables : "linked to"
    LIBFLEETCREDITS_CRYPTO ||--o{ all_executables : "linked to"
    LIBFLEETCREDITS_QT ||--o{ fleetcredits_qt : "linked to"
    LIBFLEETCREDITS_WALLET ||--o{ fleetcreditsd : "linked to"
    LIBFLEETCREDITS_WALLET ||--o{ fleetcredits_qt : "linked to"
    LIBFLEETCREDITSCONSENSUS ||--o{ external_libs : "shared library"
    
    %% ==========================================
    %% RPC COMMANDS & SERVER
    %% ==========================================
    RPC_SERVER ||--o{ CONTRIBUTION_RPC : "handles"
    RPC_SERVER ||--o{ MWEB_RPC : "handles"
    RPC_SERVER ||--o{ MINING_RPC : "handles"
    RPC_SERVER ||--o{ WALLET_RPC : "handles"
    RPC_SERVER ||--o{ BLOCKCHAIN_RPC : "handles"
    RPC_SERVER ||--o{ NET_RPC : "handles"
    RPC_SERVER ||--o{ RAWTRANSACTION_RPC : "handles"
    
    CONTRIBUTION_RPC ||--|| submitcontribution : "submits contrib_tx"
    CONTRIBUTION_RPC ||--|| getcontributionstatus : "queries status"
    CONTRIBUTION_RPC ||--|| listcontributions : "lists contributions"
    
    MWEB_RPC ||--|| createpegin : "creates peg-in"
    MWEB_RPC ||--|| createpegout : "creates peg-out"
    MWEB_RPC ||--|| routecontributiontomweb : "routes contrib to MWEB"
    MWEB_RPC ||--|| listmwebtxs : "lists MWEB transactions"
    
    MINING_RPC ||--|| generatetoaddress : "generates blocks"
    MINING_RPC ||--|| getnewaddress : "gets new address"
    MINING_RPC ||--|| getblockcount : "gets block count"
    MINING_RPC ||--|| getbalance : "gets balance"
    
    %% ==========================================
    %% QT GUI COMPONENTS
    %% ==========================================
    FleetCreditsGUI ||--|| ClientModel : "has"
    FleetCreditsGUI ||--|| WalletModel : "has"
    FleetCreditsGUI ||--|| OptionsModel : "has"
    FleetCreditsGUI ||--|| WalletFrame : "has"
    FleetCreditsGUI ||--|| ThemeManager : "has"
    
    ClientModel ||--|| CNode : "tracks network"
    ClientModel ||--|| CBlockIndex : "tracks blockchain"
    
    WalletModel ||--|| CWALLET : "wraps"
    WalletModel ||--|| TransactionTableModel : "has"
    WalletModel ||--|| RecentRequestsTableModel : "has"
    WalletModel ||--|| AddressTableModel : "has"
    
    FleetCreditsUnits ||--|| CAmount : "formats"
    FleetCreditsUnits ||--|| MFC : "Mega-Fleet Credits"
    FleetCreditsUnits ||--|| kFC : "Kilo-Fleet Credits"
    FleetCreditsUnits ||--|| FC : "Fleet Credits"
    FleetCreditsUnits ||--|| mFC : "Milli-Fleet Credits"
    FleetCreditsUnits ||--|| μFC : "Micro-Fleet Credits"
    
    FleetCreditsAddressValidator ||--|| CFleetCreditsAddress : "validates"
    FleetCreditsAmountField ||--|| FleetCreditsUnits : "uses"
    
    ThemeManager ||--|| ThemeType : "manages"
    ThemeType ||--|| FleetCredits : "default theme"
    ThemeType ||--|| Custom : "custom themes"
    
    %% ==========================================
    %% VALIDATION & PROCESSING
    %% ==========================================
    CValidationState ||--|| CBLOCK : "validates"
    CValidationState ||--|| CTRANSACTION : "validates"
    CValidationState ||--|| CMWEBEXTENSIONBLOCK : "validates"
    
    CContributionVerifier ||--|| CCONTRIBUTIONTRANSACTION : "verifies"
    CContributionVerifier ||--|| CPROOFDATA : "validates proof"
    CContributionVerifier ||--|| CORACLENODE : "uses oracles"
    CContributionVerifier ||--|| ETHICAL_CRITERIA : "references"
    CContributionVerifier ||--|| OBJECTIVE_CRITERIA : "enforces"
    
    CORACLEVOTE ||--|| VALUE_JUDGMENT_FRAMEWORK : "applies"
    CORACLEVOTE ||--|| ETHICAL_EVALUATION : "contributes to"
    
    ExtractContributionsFromBlock ||--|| CBLOCK : "parses"
    ExtractContributionsFromBlock ||--o{ CCONTRIBUTIONTRANSACTION : "extracts"
    
    ExtractContributionsFromMWEB ||--|| CMWEBEXTENSIONBLOCK : "parses"
    ExtractContributionsFromMWEB ||--o{ CCONTRIBUTIONTRANSACTION : "extracts"
    
    GetFleetCreditsBlockSubsidyWithContributions ||--|| CBLOCK : "calculates from"
    GetFleetCreditsBlockSubsidyWithContributions ||--|| CAmount : "returns reward"
    GetFleetCreditsBlockSubsidyWithContributions ||--o{ CCONTRIBUTIONTRANSACTION : "considers"
    GetFleetCreditsBlockSubsidyWithContributions ||--|| GetBonusMultiplier : "uses for calculation"
    
    GetBonusMultiplier ||--|| BONUS_LEVEL : "takes as input"
    GetBonusMultiplier ||--|| CONTRIBUTIONTYPE : "takes as input"
    GetBonusMultiplier ||--|| double : "returns multiplier"
    
    %% ==========================================
    %% NETWORK & PEER MANAGEMENT
    %% ==========================================
    CNode ||--|| CNetAddr : "has address"
    CNode ||--|| CSocket : "has socket"
    CNode ||--|| CMessageHeader : "handles messages"
    
    CAddrMan ||--o{ CNetAddr : "manages"
    CAddrMan ||--|| CAddress : "tracks"
    
    CConnman ||--o{ CNode : "manages connections"
    CConnman ||--|| CNetAddr : "handles networking"
    
    %% ==========================================
    %% MINING & CONSENSUS
    %% ==========================================
    CMiner ||--|| CBLOCK : "creates"
    CMiner ||--|| CPureBlockHeader : "solves PoW"
    CMiner ||--|| CScript : "creates coinbase"
    
    CalculateFleetCreditsNextWorkRequired ||--|| CBlockIndex : "calculates from"
    CalculateFleetCreditsNextWorkRequired ||--|| uint256 : "returns difficulty"
    
    CheckProofOfWork ||--|| uint256 : "validates hash"
    CheckProofOfWork ||--|| uint32_t : "validates nBits"
    CheckProofOfWork ||--|| CONSENSUS_PARAMS : "uses powLimit"
    CheckProofOfWork ||--|| GENESIS_BLOCK_MINING : "validates genesis"
    CheckProofOfWork ||--|| CMiner : "validates mined blocks"
    
    GetPoWHash ||--|| CBLOCKHEADER : "from block header"
    GetPoWHash ||--|| ScryptHash : "uses Scrypt algorithm"
    GetPoWHash ||--|| uint256 : "returns hash"
    
    %% ==========================================
    %% MEMPOOL & TRANSACTION POOL
    %% ==========================================
    CTxMemPool ||--o{ CTRANSACTION : "stores"
    CTxMemPool ||--o{ CCONTRIBUTIONTRANSACTION : "stores"
    CTxMemPool ||--|| CFeeRate : "uses for fee calculation"
    
    %% ==========================================
    %% SIGNAL/SLOT SYSTEM (UI Communication)
    %% ==========================================
    CClientUIInterface ||--|| ThreadSafeMessageBox : "SafeBoolCombiner"
    CClientUIInterface ||--|| ThreadSafeQuestion : "SafeBoolCombiner"
    CClientUIInterface ||--|| InitMessage : "signal"
    CClientUIInterface ||--|| ShowProgress : "signal"
    CClientUIInterface ||--|| LoadWallet : "signal"
    
    SafeBoolCombiner ||--|| boost::signals2 : "prevents no_slots_error"
    
    %% ==========================================
    %% SERIALIZATION
    %% ==========================================
    CDataStream ||--|| READWRITE : "serializes"
    CDataStream ||--|| ADD_SERIALIZE_METHODS : "uses"
    
    CCONTRIBUTIONTRANSACTION ||--|| CDataStream : "serializes to"
    CMWEBTRANSACTION ||--|| CDataStream : "serializes to"
    CVERIFICATIONRECORD ||--|| CDataStream : "serializes to"
    
    %% ==========================================
    %% CONFIGURATION & CONSTANTS
    %% ==========================================
    CONFIG_SYSTEM ||--|| FLEETCREDITS_CONF_FILENAME : "\"fleetcredits.conf\""
    CONFIG_SYSTEM ||--|| FLEETCREDITS_PID_FILENAME : "\"fleetcreditsd.pid\""
    CONFIG_SYSTEM ||--|| FLEETCREDITS_CONFIG_H : "fleetcredits-config.h"
    
    HelpMessageMode ||--|| HMM_FLEETCREDITSD : "daemon help mode"
    HelpMessageMode ||--|| HMM_FLEETCREDITS_QT : "Qt GUI help mode"
    
    %% ==========================================
    %% CONSENSUS LIBRARY API
    %% ==========================================
    FLEETCREDITSCONSENSUS_API ||--|| fleetcreditsconsensus_verify_script : "C function"
    FLEETCREDITSCONSENSUS_API ||--|| fleetcreditsconsensus_verify_script_with_amount : "C function"
    FLEETCREDITSCONSENSUS_API ||--|| fleetcreditsconsensus_version : "returns API version"
    FLEETCREDITSCONSENSUS_API ||--|| FLEETCREDITSCONSENSUS_API_VER : "version constant = 1"
    
    FLEETCREDITSCONSENSUS_API ||--|| fleetcreditsconsensus_error : "error enum"
    fleetcreditsconsensus_error ||--|| fleetcreditsconsensus_ERR_OK : "0"
    fleetcreditsconsensus_error ||--|| fleetcreditsconsensus_ERR_TX_INDEX : "1"
    fleetcreditsconsensus_error ||--|| fleetcreditsconsensus_ERR_TX_SIZE_MISMATCH : "2"
    fleetcreditsconsensus_error ||--|| fleetcreditsconsensus_ERR_TX_DESERIALIZE : "3"
    fleetcreditsconsensus_error ||--|| fleetcreditsconsensus_ERR_AMOUNT_REQUIRED : "4"
    fleetcreditsconsensus_error ||--|| fleetcreditsconsensus_ERR_INVALID_FLAGS : "5"
    
    %% ==========================================
    %% HEADER GUARDS & INCLUDE SYSTEM
    %% ==========================================
    HEADER_GUARDS ||--|| FLEETCREDITS_MODULE_NAME_H : "naming convention"
    HEADER_GUARDS ||--|| FLEETCREDITS_SCRIPT_SCRIPT_H : "script/script.h"
    HEADER_GUARDS ||--|| FLEETCREDITS_WALLET_WALLET_H : "wallet/wallet.h"
    HEADER_GUARDS ||--|| FLEETCREDITS_INIT_H : "init.h"
    HEADER_GUARDS ||--|| FLEETCREDITS_UTIL_H : "util.h"
    
    FLEETCREDITS_INCLUDES ||--|| FLEETCREDITS_CONFIG_INCLUDES : "config headers"
    FLEETCREDITS_INCLUDES ||--|| FLEETCREDITS_QT_INCLUDES : "Qt headers"
    
    %% ==========================================
    %% TESTING INFRASTRUCTURE
    %% ==========================================
    TestingSetup ||--|| CCHAINPARAMS : "initializes"
    TestingSetup ||--|| CWALLET : "creates test wallet"
    
    MWEBTest ||--|| CMWEBTRANSACTION : "creates test"
    MWEBTest ||--|| CMWEBEXTENSIONBLOCK : "creates test"
    MWEBTest ||--|| CPegInTransaction : "creates test"
    MWEBTest ||--|| CPegOutTransaction : "creates test"
    
    test_fleetcredits_qt ||--|| FleetCreditsGUI : "tests Qt components"
    test_fleetcredits_qt ||--|| LIBFLEETCREDITS_QT : "links against"
    
    %% ==========================================
    %% COMPLETE ENTITY ATTRIBUTE DEFINITIONS
    %% ==========================================
    %% This section provides FULL attribute lists for ALL entities
    %% No entity only shows "id" - all attributes are fully defined
    
    %% ==========================================
    %% CORE BLOCKCHAIN ENTITIES
    %% ==========================================
    
    CBLOCK {
        vector<CTransactionRef> vtx "all transactions in block"
        shared_ptr<CMWEBExtensionBlock> mweb_extension "optional MWEB extension"
        bool fChecked "memory-only validation flag"
    }
    
    CBLOCKHEADER {
        int nVersion "block version (includes AuxPow flag)"
        uint256 hashPrevBlock "previous block hash"
        uint256 hashMerkleRoot "merkle root of transactions"
        unsigned int nTime "block timestamp"
        unsigned int nBits "difficulty target"
        unsigned int nNonce "proof-of-work nonce"
        shared_ptr<CAuxPow> auxpow "auxiliary proof-of-work (optional)"
    }
    
    CPUREBLOCKHEADER {
        int nVersion "base block version"
        uint256 hashPrevBlock "previous block hash"
        uint256 hashMerkleRoot "merkle root"
        unsigned int nTime "timestamp"
        unsigned int nBits "difficulty"
        unsigned int nNonce "nonce"
    }
    
    CBLOCKINDEX {
        const uint256* phashBlock "pointer to block hash"
        CBlockIndex* pprev "previous block index"
        CBlockIndex* pskip "skip list pointer"
        int nHeight "block height (genesis = 0)"
        int nFile "block file number"
        unsigned int nDataPos "byte offset in block file"
        unsigned int nUndoPos "byte offset in undo file"
        arith_uint256 nChainWork "cumulative chain work"
        unsigned int nTx "transaction count in block"
        unsigned int nChainTx "cumulative transaction count"
        unsigned int nStatus "validation status flags"
        int nVersion "block version"
        uint256 hashMerkleRoot "merkle root"
        unsigned int nTime "timestamp"
        unsigned int nBits "difficulty"
        unsigned int nNonce "nonce"
        int32_t nSequenceId "receive order id"
        unsigned int nTimeMax "max time in chain"
    }
    
    CTRANSACTION {
        int32_t nVersion "transaction version"
        vector<CTxIn> vin "transaction inputs"
        vector<CTxOut> vout "transaction outputs"
        uint32_t nLockTime "lock time"
        CScriptWitness wit "witness data"
    }
    
    CTXIN {
        COutPoint prevout "previous output reference"
        CScript scriptSig "input script"
        uint32_t nSequence "sequence number"
        CScriptWitness scriptWitness "witness data"
    }
    
    CTXOUT {
        CAmount nValue "output amount in FC"
        CScript scriptPubKey "output script"
    }
    
    COUTPOINT {
        uint256 hash "transaction hash"
        uint32_t n "output index"
    }
    
    UTXO {
        COutPoint outpoint "spending reference"
        CTxOut txout "output data"
        int nHeight "block height created"
        bool fCoinBase "is coinbase"
        int nVersion "transaction version"
    }
    
    CAUXPOW {
        CAuxPowHeader header "auxpow header"
        vector<CTransactionRef> vtx "parent chain transactions"
        CBlockHeader parentBlock "parent block header"
    }
    
    MERKLE_ROOT {
        uint256 hash "merkle root hash"
        vector<uint256> vHashes "transaction hashes"
        bool fMutations "tree mutations detected"
    }
    
    %% ==========================================
    %% CONTRIBUTION SYSTEM ENTITIES
    %% ==========================================
    
    CCONTRIBUTIONTRANSACTION {
        uint256 tx_id PK "transaction ID"
        CPubKey contributor FK "contributor public key"
        CFleetCreditsAddress reward_address FK "address to receive reward"
        ContributionType contrib_type "contribution type enum"
        CProofData proof_data "proof data structure"
        uint64_t timestamp "submission timestamp"
        uint32_t bonus_level "bonus level (0-4)"
        bool requires_mweb "MWEB required flag"
        vector<unsigned char> signature "contributor signature"
        string ethical_review_status "ethical review status"
    }
    
    CPROOFDATA {
        uint256 hash "proof data hash"
        ContributionType contrib_type "contribution type"
        vector<unsigned char> evidence "proof evidence (IPFS/GitHub/etc)"
        vector<unsigned char> nonce "unique nonce"
        uint64_t timestamp "proof timestamp"
        vector<unsigned char> metadata "additional metadata"
    }
    
    CCODECONTRIBUTIONTRANSACTION {
        string repo_url "GitHub repository URL"
        uint256 commit_hash "commit hash"
        uint32_t lines_changed "lines changed (>10 required)"
        string language "programming language"
        bool passes_lint "lint check passed"
        uint32_t account_age_days "account age (>30 required)"
    }
    
    CMENTORSHIPTRANSACTION {
        CPubKey mentor "mentor public key"
        CPubKey mentee "mentee public key"
        vector<unsigned char> mentor_signature "mentor signature"
        vector<unsigned char> mentee_signature "mentee signature"
        ApprovalStatus approval_status "approval status enum"
        uint256 feedback_hash "feedback hash"
        uint256 improvement_commit_hash "improvement commit hash"
    }
    
    CAIVALIDATIONTRANSACTION {
        string ai_model_id "AI model identifier"
        float accuracy_score "accuracy score (>=0.8 required)"
        uint256 validation_report_hash "validation report hash"
        uint256 human_override_hash "human override hash"
        bool human_override_verified "human override verified"
    }
    
    CDATALABELINGTRANSACTION {
        string dataset_id "dataset identifier"
        uint32_t labels_count "number of labels"
        uint256 labels_hash "IPFS hash of labels"
        float inter_annotator_agreement "agreement score (>=0.7)"
        uint32_t annotator_count "annotator count (>=3)"
    }
    
    CETHICALREVIEWTRANSACTION {
        uint256 rationale_hash "ethical rationale hash"
        uint64_t rationale_length "rationale length (>=100 words)"
        bool oracle_consensus_achieved "consensus achieved flag"
    }
    
    CCREATIVEWORKTRANSACTION {
        uint256 ipfs_hash "IPFS hash of creative work"
        string work_type "art, writing, music, etc"
        string media_type "image, video, audio, text"
        uint64_t creation_date "work creation date"
    }
    
    CEDUCATIONALCONTENTTRANSACTION {
        uint256 content_hash "educational content hash"
        string content_type "course, tutorial, documentation"
        string subject_area "subject area"
        uint32_t learner_count "number of learners"
    }
    
    CCHARITABLEACTTRANSACTION {
        string charity_name "charity organization name"
        uint256 donation_proof_hash "donation proof hash"
        CAmount donation_amount "donation amount"
        string charity_url "charity website URL"
    }
    
    CONTRIBUTIONTYPE {
        uint8_t value "enum value (0x01-0x08)"
    }
    
    CODE_CONTRIBUTION {
        uint8_t value "0x01"
    }
    
    CHARITABLE_ACT {
        uint8_t value "0x02"
    }
    
    CREATIVE_WORK {
        uint8_t value "0x03"
    }
    
    EDUCATIONAL_CONTENT {
        uint8_t value "0x04"
    }
    
    CODE_MENTORSHIP {
        uint8_t value "0x05"
    }
    
    AI_VALIDATION {
        uint8_t value "0x06"
    }
    
    DATA_LABELING {
        uint8_t value "0x07"
    }
    
    ETHICAL_REVIEW {
        uint8_t value "0x08"
    }
    
    %% ==========================================
    %% MWEB ENTITIES
    %% ==========================================
    
    CMWEBEXTENSIONBLOCK {
        uint256 prev_mweb_hash PK "previous MWEB extension hash"
        vector<CMWEBTransaction> mweb_txs "MWEB transactions"
        vector<CPegInTransaction> peg_ins "peg-in transactions"
        vector<CPegOutTransaction> peg_outs "peg-out transactions"
        uint256 mweb_root "MWEB UTXO set merkle root"
        uint256 extension_block_hash "extension block hash"
    }
    
    CMWEBTRANSACTION {
        vector<CMWEBInput> inputs "MWEB inputs"
        vector<CMWEBOutput> outputs "MWEB outputs"
        vector<CMWEBKernel> kernels "MWEB kernels"
        uint64_t lock_height "minimum block height"
        uint256 tx_hash PK "transaction hash"
    }
    
    CMWEBINPUT {
        CPedersenCommitment output_commitment "reference to output commitment"
        vector<unsigned char> witness "witness data proving ownership"
    }
    
    CMWEBOUTPUT {
        CPedersenCommitment commitment "Pedersen commitment to value"
        CRangeProof range_proof "proof value is non-negative"
        vector<unsigned char> view_key "optional view key for disclosure"
    }
    
    CPEDERSENCOMMITMENT {
        CPubKey commitment PK "commitment = vG + rH (33 bytes)"
        uint256 blinding_factor_hash "hash of blinding factor"
    }
    
    CRANGEPROOF {
        vector<unsigned char> proof_data "bulletproofs range proof"
        uint32_t value_range_min "minimum value (typically 0)"
        uint32_t value_range_max "maximum value (MAX_MONEY)"
    }
    
    CMWEBKERNEL {
        CPubKey excess_value PK "excess value public key"
        vector<unsigned char> signature "kernel signature"
        uint64_t fee "fee in FC"
        uint256 kernel_id "unique kernel identifier"
    }
    
    CPEGINTRANSACTION {
        uint256 main_chain_tx_id FK "main chain transaction ID"
        CPubKey recipient_address "MWEB recipient address"
        uint64_t amount "amount in FC (visible)"
        uint256 peg_tx_id PK "unique peg identifier"
    }
    
    CPEGOUTTRANSACTION {
        CPedersenCommitment mweb_input_commitment "MWEB input commitment"
        CPubKey main_chain_address "main chain recipient address"
        uint64_t amount "amount in FC (visible)"
        vector<unsigned char> view_key "view key for proof"
        uint256 peg_tx_id PK "unique peg identifier"
    }
    
    CVIEWKEY {
        CPubKey view_public_key PK "public view key"
        vector<unsigned char> view_private_key "private view key (encrypted)"
        uint256 view_key_hash "hash of view key pair"
        CPubKey scan_pubkey "scan public key"
    }
    
    MWEB_WALLET {
        CWallet* parent_wallet FK "parent wallet"
        vector<CMWEBOutput> mweb_utxos "tracked MWEB UTXOs"
        CViewKey view_key "view key for wallet"
        map<CPedersenCommitment, uint64_t> commitment_map "commitment to amount map"
    }
    
    %% ==========================================
    %% VERIFICATION & ORACLE ENTITIES
    %% ==========================================
    
    CVERIFICATIONRECORD {
        uint256 record_id PK "unique verification record ID"
        uint256 contrib_tx_id FK "contribution transaction ID"
        CPubKey contributor_id FK "contributor public key"
        uint32_t oracle_vote_count "current vote count"
        uint32_t required_votes "required votes (typically 3)"
        float consensus_score "consensus score (0.0-1.0)"
        bool approved "approval status"
        VerificationStatus verification_status "verification status enum"
        uint64_t verification_timestamp "verification timestamp"
        string ethical_review_status "ethical review status"
    }
    
    CORACLENODE {
        CPubKey pubkey PK "oracle public key"
        string node_address "network node address"
        int64_t stake_fc "staked FC (>=500k required)"
        int32_t reputation_score "reputation score"
        uint32_t total_votes_cast "total votes cast"
        uint32_t correct_votes "correct votes"
        uint32_t incorrect_votes "incorrect votes"
        bool is_active "active status"
        uint64_t last_rotation_date "last rotation timestamp"
        uint64_t last_vote_time "last vote timestamp"
        uint32_t missed_votes "missed vote count"
    }
    
    CORACLEVOTE {
        uint256 vote_id PK "unique vote ID"
        CPubKey oracle_pubkey FK "oracle public key"
        uint256 verification_record_id FK "verification record ID"
        OracleVoteChoice vote_choice "vote choice enum"
        uint64_t vote_timestamp "vote timestamp"
        vector<unsigned char> vote_signature "vote signature"
        string vote_reason "vote reason/justification"
    }
    
    ORACLEVOTECHOICE {
        uint8_t value "enum value"
    }
    
    VOTE_APPROVE {
        uint8_t value "0x01"
    }
    
    VOTE_REJECT {
        uint8_t value "0x02"
    }
    
    VOTE_ABSTAIN {
        uint8_t value "0x03"
    }
    
    VERIFICATIONSTATUS {
        uint8_t value "enum value"
    }
    
    VERIFICATION_PENDING {
        uint8_t value "0x01"
    }
    
    VERIFICATION_APPROVED {
        uint8_t value "0x02"
    }
    
    VERIFICATION_REJECTED {
        uint8_t value "0x03"
    }
    
    VERIFICATION_CHALLENGED {
        uint8_t value "0x04"
    }
    
    VALIDATORPOOL {
        uint256 pool_id PK "validator pool ID"
        int32_t min_reputation "minimum reputation required"
        uint32_t member_count "current member count"
        int64_t total_stake "total staked FC"
        float consensus_score "pool consensus score"
        bool is_active "active status"
        uint64_t creation_date "pool creation date"
    }
    
    HUMANVALIDATOR {
        uint256 validator_id PK "validator ID"
        CFleetCreditsAddress address FK "validator address"
        int32_t reputation "reputation score"
        uint32_t verified_contributions "verified contribution count"
        int64_t stake "staked FC"
        uint64_t join_date "join timestamp"
        uint32_t votes_cast "total votes cast"
    }
    
    HUMANVALIDATORVOTE {
        uint256 vote_id PK "vote ID"
        CPubKey validator_pubkey FK "validator public key"
        uint256 verification_record_id FK "verification record ID"
        bool approval "approval vote"
        uint64_t vote_timestamp "vote timestamp"
        string vote_comment "vote comment"
    }
    
    INTER_ANNOTATOR_AGREEMENT {
        uint256 agreement_id PK "agreement ID"
        float agreement_score "agreement score (>=0.7)"
        uint32_t annotator_count "annotator count (>=3)"
        uint256 contribution_id FK "contribution transaction ID"
        uint64_t agreement_timestamp "agreement timestamp"
    }
    
    SLASHING_RECORD {
        uint256 slashing_id PK "slashing record ID"
        CPubKey oracle_pubkey FK "oracle public key"
        uint256 bad_vote_id FK "bad vote ID"
        CAmount slashed_amount "slashed amount (50% of stake)"
        uint64_t slashing_timestamp "slashing timestamp"
        string slashing_reason "reason for slashing"
    }
    
    ETHICAL_EVALUATION {
        uint256 evaluation_id PK "evaluation ID"
        uint256 contrib_tx_id FK "contribution transaction ID"
        CPubKey contributor_id FK "contributor public key"
        CPubKey evaluator_id FK "evaluator oracle public key"
        ContributionType contrib_type FK "contribution type"
        string ethical_status "ethical status string"
        float impact_score "impact score (0.0-1.0)"
        float authenticity_score "authenticity score (0.0-1.0)"
        float alignment_score "alignment score (0.0-1.0)"
        float harm_prevention_score "harm prevention score (0.0-1.0)"
        uint64_t evaluation_timestamp "evaluation timestamp"
    }
    
    ETHICAL_CRITERIA {
        string criteria_id PK "criteria ID"
        ContributionType contrib_type FK "contribution type"
        string objective_requirements "objective requirements"
        string value_judgment_guidelines "value judgment guidelines"
        string good_examples "examples of good contributions"
        string problematic_examples "examples of problematic contributions"
        string ethical_guidelines_reference "reference to guidelines"
    }
    
    OBJECTIVE_CRITERIA {
        string criterion_id PK "criterion ID"
        bool valid_proof_required "valid proof required"
        bool non_duplicate_required "non-duplicate required"
        uint32_t minimum_threshold "minimum threshold value"
        string validation_function "validation function name"
    }
    
    VALUE_JUDGMENT_FRAMEWORK {
        string framework_id PK "framework ID"
        string principle_name "principle name"
        string evaluation_guidelines "evaluation guidelines"
        float weight "weight in calculation"
    }
    
    IMPACT_ASSESSMENT {
        uint256 assessment_id PK "assessment ID"
        float positive_impact_score "positive impact score"
        float negative_impact_score "negative impact score"
        uint32_t affected_people_count "affected people count"
    }
    
    AUTHENTICITY_CHECK {
        uint256 check_id PK "check ID"
        bool proof_verified "proof verified"
        bool source_verified "source verified"
        bool timestamp_verified "timestamp verified"
    }
    
    ALIGNMENT_WITH_PUBLIC_GOOD {
        uint256 alignment_id PK "alignment ID"
        float public_good_score "public good score"
        bool contributes_to_commons "contributes to commons"
    }
    
    HARM_PREVENTION {
        uint256 prevention_id PK "prevention ID"
        bool potential_harm_detected "harm detected"
        bool harm_mitigated "harm mitigated"
        string harm_description "harm description"
    }
    
    VALID_PROOF {
        bool proof_exists "proof exists"
        bool proof_verifiable "proof verifiable"
        uint256 proof_hash "proof hash"
    }
    
    NON_DUPLICATE {
        bool is_unique "is unique"
        uint256 duplicate_check_hash "duplicate check hash"
    }
    
    MINIMUM_THRESHOLDS {
        uint32_t threshold_value "threshold value"
        string threshold_type "threshold type"
    }
    
    APPROVALSTATUS {
        uint8_t value "enum value"
    }
    
    APPROVED {
        uint8_t value "0x01"
    }
    
    REJECTED {
        uint8_t value "0x02"
    }
    
    NEEDS_WORK {
        uint8_t value "0x03"
    }
    
    %% ==========================================
    %% WALLET & ADDRESS ENTITIES
    %% ==========================================
    
    CWALLET {
        CKeyStore keyStore "key store"
        map<uint256, CWalletTx> mapWallet "wallet transactions"
        int64_t nTimeFirstKey "first key creation time"
        string strWalletFile "wallet file path"
        int nWalletVersion "wallet version"
        int nWalletMaxVersion "max wallet version"
        int64_t nNextResend "next resend time"
        int64_t nLastResend "last resend time"
        bool fBroadcastTransactions "broadcast flag"
        TxSpends mapTxSpends "spent transactions map"
        CHDChain hdChain "HD chain data"
        bool fFileBacked "file backed flag"
        set<int64_t> setKeyPool "key pool set"
        map<CTxDestination, CKeyMetadata> mapKeyMetadata "key metadata"
        MasterKeyMap mapMasterKeys "master keys"
        unsigned int nMasterKeyMaxID "max master key ID"
        list<CAccountingEntry> laccentries "accounting entries"
        TxItems wtxOrdered "ordered transactions"
        int64_t nOrderPosNext "next order position"
        map<CTxDestination, CAddressBookData> mapAddressBook "address book"
        CPubKey vchDefaultKey "default key"
        set<COutPoint> setLockedCoins "locked coins"
    }
    
    CWALLETTX {
        const CWallet* pwallet "parent wallet"
        mapValue_t mapValue "transaction metadata"
        vector<pair<string, string>> vOrderForm "order form data"
        unsigned int fTimeReceivedIsTxTime "time received flag"
        unsigned int nTimeReceived "time received"
        unsigned int nTimeSmart "smart time"
        bool fFromMe "from me flag"
        string strFromAccount "from account"
        int64_t nOrderPos "order position"
        mutable bool fDebitCached "debit cached flag"
        mutable bool fCreditCached "credit cached flag"
        mutable CAmount nDebitCached "cached debit"
        mutable CAmount nCreditCached "cached credit"
    }
    
    CWALLETDB {
        CDB db "Berkeley DB database"
        string filename "database filename"
    }
    
    CFleetCreditsAddress {
        vector<unsigned char> vchVersion PK "version bytes"
        vector<unsigned char> vchData "address data"
    }
    
    CFleetCreditsSecret {
        CKey vchSecret "secret key"
        bool fCompressed "compressed flag"
    }
    
    CKey {
        unsigned char vch "32-byte private key"
        bool fValid "valid flag"
        bool fCompressed "compressed flag"
    }
    
    CPubKey {
        unsigned char vch "33-byte public key"
        bool fValid "valid flag"
    }
    
    CFleetCreditsExtKey {
        CExtKey key "extended key"
    }
    
    CFleetCreditsExtPubKey {
        CExtPubKey pubkey "extended public key"
    }
    
    REPUTATION_SCORE {
        CFleetCreditsAddress address FK "address"
        int32_t score "reputation score"
        uint32_t contributions "contribution count"
        uint64_t last_update "last update timestamp"
    }
    
    VERIFIED_IDENTITY {
        CFleetCreditsAddress address FK "address"
        string identity_type "GitHub, DID, etc"
        string identity_id "identity identifier"
        uint256 verification_hash "verification hash"
    }
    
    STAKING_RECORD {
        CFleetCreditsAddress address FK "staker address"
        CAmount stake_amount "staked amount"
        uint64_t stake_start "stake start time"
        uint64_t stake_end "stake end time"
        CAmount reward_accrued "accrued reward"
    }
    
    %% ==========================================
    %% FEE & REWARD ENTITIES
    %% ==========================================
    
    TRANSACTION_FEE {
        CAmount fee_amount "fee amount"
        CFeeRate fee_rate FK "fee rate"
        bool is_zero_fee "zero fee flag"
    }
    
    CFeeRate {
        CAmount nSatoshisPerK "satoshis per KB"
        CAmount fee_per_kb PK "fee per KB"
    }
    
    MICRO_TX_THRESHOLD {
        CAmount value PK "1000 * COIN"
    }
    
    ZERO_FEE {
        CAmount amount "0"
    }
    
    SCALABLE_FEE {
        CFeeRate fee_rate FK "scalable fee rate"
        CAmount min_fee "minimum fee"
    }
    
    BLOCK_SUBSIDY {
        CAmount base_reward PK "10000 * COIN"
        CAmount bonus_amount "bonus amount"
    }
    
    BASE_REWARD {
        CAmount value PK "10000 * COIN"
    }
    
    BONUS_MULTIPLIER {
        double multiplier "multiplier value (1.0-1.25)"
        BONUS_LEVEL bonus_level FK "bonus level"
        ContributionType contrib_type FK "contribution type"
    }
    
    BONUS_LEVEL {
        uint32_t value "level value (0-4)"
    }
    
    BONUS_NONE {
        uint32_t value "0 (1.0x multiplier)"
    }
    
    BONUS_LOW {
        uint32_t value "1 (1.05x multiplier)"
    }
    
    BONUS_MEDIUM {
        uint32_t value "2 (1.10x multiplier)"
    }
    
    BONUS_HIGH {
        uint32_t value "3 (1.15x multiplier)"
    }
    
    BONUS_CRITICAL {
        uint32_t value "4 (1.20x-1.25x multiplier)"
    }
    
    BLOCK_REWARD {
        CAmount total_reward "total reward amount"
        CAmount base_subsidy FK "base subsidy"
        CAmount bonus_amount "bonus amount"
    }
    
    COINBASE_TX {
        uint256 tx_id PK "transaction ID"
        CAmount block_reward "block reward amount"
        CFleetCreditsAddress miner_address "miner address"
        vector<CFleetCreditsAddress> contributor_addresses "contributor addresses"
    }
    
    REWARD_PAYOUT {
        uint256 payout_id PK "payout ID"
        uint256 contrib_tx_id FK "contribution transaction ID"
        CFleetCreditsAddress recipient FK "recipient address"
        CAmount reward_amount "reward amount"
        uint32_t bonus_level "bonus level applied"
        uint64_t payout_timestamp "payout timestamp"
    }
    
    CONTRIBUTOR_ADDRESS {
        CFleetCreditsAddress address PK "contributor address"
        CPubKey pubkey FK "public key"
    }
    
    COMMUNITY_RESERVE {
        CAmount balance_fc PK "reserve balance"
        MULTISIG_WALLET multisig_wallet FK "multisig wallet"
        CAmount total_allocated "total allocated"
        uint64_t last_spend "last spend timestamp"
    }
    
    MULTISIG_WALLET {
        uint256 wallet_id PK "wallet ID"
        vector<CPubKey> signers "multisig signers"
        uint32_t required_signatures "required signatures"
        CAmount balance "wallet balance"
    }
    
    %% ==========================================
    %% NETWORK & CHAIN PARAMETERS
    %% ==========================================
    
    CCHAINPARAMS {
        string strNetworkID PK "network ID"
        CMessageHeader header "message header"
        int nDefaultPort "default P2P port"
        vector<CDNSSeedData> vSeeds "DNS seeds"
        CCheckpointData checkpointData "checkpoint data"
        ChainTxData chainTxData "chain transaction data"
        CBlock genesis "genesis block"
        CONSENSUS_PARAMS consensus FK "consensus parameters"
    }
    
    CONSENSUS_PARAMS {
        uint256 hashGenesisBlock PK "genesis block hash"
        CAmount nSubsidy "10000 * COIN"
        int64_t nTargetTimespan "60 seconds"
        int64_t nTargetSpacing "60 seconds"
        uint256 nMinimumChainWork "minimum chain work"
        uint256 defaultAssumeValid "default assume valid"
        uint256 powLimit "proof-of-work limit"
        bool fPowAllowMinDifficultyBlocks "allow min difficulty"
        bool fPowNoRetargeting "no retargeting"
        int64_t nPowTargetSpacing "target spacing"
        int64_t nPowTargetTimespan "target timespan"
    }
    
    MAINNET {
        int p2p_port "22556"
        int rpc_port "22555"
    }
    
    TESTNET {
        int p2p_port "44556"
        int rpc_port "44555"
    }
    
    REGTEST {
        int p2p_port "18444"
        int rpc_port "18332"
    }
    
    CMessageHeader {
        unsigned char pchMessageStart_0 "0xfc"
        unsigned char pchMessageStart_1 "0xc1"
        unsigned char pchMessageStart_2 "0xb7"
        unsigned char pchMessageStart_3 "0xdc"
    }
    
    CCheckpointData {
        map<int, uint256> mapCheckpoints "checkpoint map"
        uint256 hashGenesisBlock "genesis block hash at height 0"
    }
    
    CDNSSeedData {
        string host "DNS host"
        uint16_t port "port number"
    }
    
    ChainTxData {
        uint256 nTxCount "transaction count"
        int64_t nTime "time"
        double dTxRate "transaction rate"
    }
    
    %% ==========================================
    %% NETWORK & PEER ENTITIES
    %% ==========================================
    
    CNode {
        CNetAddr addr "network address"
        CSocket hSocket "socket handle"
        CMessageHeader hdr "message header"
        uint64_t nServices "service flags"
        int64_t nLastSend "last send time"
        int64_t nLastRecv "last receive time"
        int64_t nTimeConnected "connection time"
        int nVersion "protocol version"
        string strSubVer "sub-version string"
        bool fInbound "inbound connection"
        bool fWhitelisted "whitelisted"
    }
    
    CNetAddr {
        unsigned char ip "IP address (16 bytes)"
        uint16_t port "port number"
    }
    
    CAddress {
        CNetAddr addr "network address"
        uint64_t nServices "service flags"
        uint32_t nTime "timestamp"
    }
    
    CAddrMan {
        vector<CAddress> vRandom "random addresses"
        int nIdCount "ID counter"
        map<int, CAddress> mapAddr "address map"
    }
    
    CConnman {
        vector<CNode*> vNodes "connected nodes"
        CAddrMan addrman "address manager"
        bool fNetworkActive "network active flag"
    }
    
    CSocket {
        int fd "file descriptor"
    }
    
    %% ==========================================
    %% SCRIPT & CRYPTO ENTITIES
    %% ==========================================
    
    CScript {
        vector<unsigned char> data "script data"
    }
    
    CScriptWitness {
        vector<vector<unsigned char>> stack "witness stack"
    }
    
    CAmount {
        int64_t value PK "amount in koinu"
    }
    
    OP_RETURN_DATA {
        vector<unsigned char> data "OP_RETURN data"
        unsigned char marker_0 "0xFC"
        unsigned char marker_1 "0x01"
    }
    
    %% ==========================================
    %% MINING ENTITIES
    %% ==========================================
    
    CMiner {
        CBlock template_block "block template"
        unsigned int nNonce "current nonce"
        CAmount blockReward "block reward"
        vector<CTransactionRef> block_txs "block transactions"
    }
    
    GENESIS_BLOCK_MINING {
        CBlock genesis_block "genesis block"
        uint32_t nNonce "nonce value"
        uint256 hashResult "hash result"
        bool fValidPoW "valid PoW flag"
    }
    
    GENESIS_BLOCK_VALIDATION {
        CBlock genesis_block "genesis block"
        uint256 expected_hash "expected hash"
        bool fValid "validation result"
    }
    
    %% ==========================================
    %% RPC ENTITIES
    %% ==========================================
    
    RPC_SERVER {
        string rpc_user "RPC username"
        string rpc_password "RPC password"
        bool fRPCRunning "RPC running flag"
    }
    
    CONTRIBUTION_RPC {
        string submitcontribution "submitcontribution RPC"
        string getcontributionstatus "getcontributionstatus RPC"
        string listcontributions "listcontributions RPC"
    }
    
    MWEB_RPC {
        string createpegin "createpegin RPC"
        string createpegout "createpegout RPC"
        string routecontributiontomweb "routecontributiontomweb RPC"
        string listmwebtxs "listmwebtxs RPC"
    }
    
    MINING_RPC {
        string generatetoaddress "generatetoaddress RPC"
        string getnewaddress "getnewaddress RPC"
        string getblockcount "getblockcount RPC"
        string getbalance "getbalance RPC"
    }
    
    WALLET_RPC {
        string sendtoaddress "sendtoaddress RPC"
        string getbalance "getbalance RPC"
        string listtransactions "listtransactions RPC"
    }
    
    BLOCKCHAIN_RPC {
        string getblock "getblock RPC"
        string gettxout "gettxout RPC"
        string getblockhash "getblockhash RPC"
    }
    
    NET_RPC {
        string getnetworkinfo "getnetworkinfo RPC"
        string getpeerinfo "getpeerinfo RPC"
    }
    
    RAWTRANSACTION_RPC {
        string createrawtransaction "createrawtransaction RPC"
        string signrawtransaction "signrawtransaction RPC"
        string sendrawtransaction "sendrawtransaction RPC"
    }
    
    %% ==========================================
    %% QT GUI ENTITIES
    %% ==========================================
    
    FleetCreditsGUI {
        ClientModel* clientModel "client model"
        WalletModel* walletModel "wallet model"
        OptionsModel* optionsModel "options model"
        WalletFrame* walletFrame "wallet frame"
        ThemeManager* themeManager "theme manager"
    }
    
    ClientModel {
        CNode* node "network node"
        CBlockIndex* blockIndex "block index"
        int numBlocks "block count"
        QDateTime blockDate "block date"
    }
    
    WalletModel {
        CWallet* wallet "wallet instance"
        TransactionTableModel* transactionTableModel "transaction model"
        RecentRequestsTableModel* recentRequestsTableModel "recent requests model"
        AddressTableModel* addressTableModel "address model"
    }
    
    WalletModelTransaction {
        QList<SendCoinsRecipient> recipients "recipients"
        CAmount transactionFee "transaction fee"
        CAmount totalTransactionAmount "total amount"
    }
    
    TransactionTableModel {
        vector<TransactionRecord> records "transaction records"
        int sortColumn "sort column"
        Qt::SortOrder sortOrder "sort order"
    }
    
    AddressTableModel {
        map<CFleetCreditsAddress, CAddressBookData> addressBook "address book"
        AddressTableEntry::Type type "entry type"
    }
    
    FleetCreditsUnits {
        Unit unit "unit enum"
        CAmount amount "amount"
    }
    
    FleetCreditsAddressValidator {
        bool fAllowEmpty "allow empty"
        bool fTestNet "testnet flag"
    }
    
    FleetCreditsAmountField {
        CAmount value "amount value"
        Unit displayUnit "display unit"
    }
    
    ThemeManager {
        ThemeType currentTheme "current theme"
        map<string, Theme> themes "theme map"
    }
    
    ThemeType {
        string name "theme name"
    }
    
    %% ==========================================
    %% VALIDATION ENTITIES
    %% ==========================================
    
    CValidationState {
        enum Mode mode "validation mode"
        string rejectReason "reject reason"
        unsigned char chRejectCode "reject code"
    }
    
    CContributionVerifier {
        vector<CORACLENODE> oracles "oracle nodes"
        ETHICAL_CRITERIA criteria "ethical criteria"
        OBJECTIVE_CRITERIA objectiveCriteria "objective criteria"
    }
    
    ExtractContributionsFromBlock {
        CBlock block "block to extract from"
        vector<CContributionTransaction> contributions "extracted contributions"
    }
    
    ExtractContributionsFromMWEB {
        CMWEBExtensionBlock mweb_block "MWEB extension block"
        vector<CContributionTransaction> contributions "extracted contributions"
    }
    
    GetFleetCreditsBlockSubsidyWithContributions {
        int nHeight "block height"
        CONSENSUS_PARAMS consensusParams "consensus parameters"
        uint256 prevHash "previous hash"
        vector<CContributionTransaction> contrib_txs "contributions"
        CAmount reward "calculated reward"
    }
    
    GetBonusMultiplier {
        uint32_t bonus_level "bonus level"
        ContributionType contrib_type "contribution type"
        double multiplier "calculated multiplier"
    }
    
    CheckProofOfWork {
        uint256 hash "block hash"
        uint32_t nBits "difficulty bits"
        CONSENSUS_PARAMS consensusParams "consensus parameters"
        bool fValid "validation result"
    }
    
    GetPoWHash {
        CBLOCKHEADER blockHeader "block header"
        uint256 hashResult "hash result"
    }
    
    CalculateFleetCreditsNextWorkRequired {
        CBlockIndex* pindexLast "last block index"
        CAmount nTotalReward "total reward"
        uint256 nextDifficulty "next difficulty"
    }
    
    %% ==========================================
    %% MEMPOOL ENTITIES
    %% ==========================================
    
    CTxMemPool {
        map<uint256, CTransactionRef> mapTx "transaction map"
        map<COutPoint, CInPoint> mapNextTx "next transaction map"
        CFeeRate minRelayFee "minimum relay fee"
        uint64_t totalTxSize "total transaction size"
        uint64_t cachedInnerUsage "cached inner usage"
    }
    
    %% ==========================================
    %% BUILD SYSTEM ENTITIES
    %% ==========================================
    
    BUILD_SYSTEM {
        string LIBFLEETCREDITS_SERVER "libfleetcredits_server.a"
        string LIBFLEETCREDITS_COMMON "libfleetcredits_common.a"
        string LIBFLEETCREDITS_CONSENSUS "libfleetcredits_consensus.a"
        string LIBFLEETCREDITS_CLI "libfleetcredits_cli.a"
        string LIBFLEETCREDITS_UTIL "libfleetcredits_util.a"
        string LIBFLEETCREDITS_CRYPTO "crypto/libfleetcredits_crypto.a"
        string LIBFLEETCREDITS_QT "qt/libfleetcreditsqt.a"
        string LIBFLEETCREDITS_WALLET "libfleetcredits_wallet.a"
        string LIBFLEETCREDITS_ZMQ "libfleetcredits_zmq.a"
        string LIBFLEETCREDITSCONSENSUS "libfleetcreditsconsensus.la"
    }
    
    LIBFLEETCREDITS_SERVER {
        string library_name "libfleetcredits_server.a"
        vector<string> source_files "source files"
    }
    
    LIBFLEETCREDITS_COMMON {
        string library_name "libfleetcredits_common.a"
        vector<string> source_files "source files"
    }
    
    LIBFLEETCREDITS_CONSENSUS {
        string library_name "libfleetcredits_consensus.a"
        vector<string> source_files "source files"
    }
    
    LIBFLEETCREDITS_CLI {
        string library_name "libfleetcredits_cli.a"
        vector<string> source_files "source files"
    }
    
    LIBFLEETCREDITS_UTIL {
        string library_name "libfleetcredits_util.a"
        vector<string> source_files "source files"
    }
    
    LIBFLEETCREDITS_CRYPTO {
        string library_name "libfleetcredits_crypto.a"
        vector<string> source_files "source files"
    }
    
    LIBFLEETCREDITS_QT {
        string library_name "libfleetcreditsqt.a"
        vector<string> source_files "source files"
    }
    
    LIBFLEETCREDITS_WALLET {
        string library_name "libfleetcredits_wallet.a"
        vector<string> source_files "source files"
    }
    
    LIBFLEETCREDITS_ZMQ {
        string library_name "libfleetcredits_zmq.a"
        vector<string> source_files "source files"
    }
    
    LIBFLEETCREDITSCONSENSUS {
        string library_name "libfleetcreditsconsensus.la"
        vector<string> source_files "source files"
    }
    
    FLEETCREDITS_DAEMON {
        string executable_name "fleetcreditsd"
        vector<string> linked_libraries "linked libraries"
    }
    
    FLEETCREDITS_QT {
        string executable_name "fleetcredits-qt"
        vector<string> linked_libraries "linked libraries"
    }
    
    FLEETCREDITS_CLI {
        string executable_name "fleetcredits-cli"
        vector<string> linked_libraries "linked libraries"
    }
    
    FLEETCREDITS_TX {
        string executable_name "fleetcredits-tx"
        vector<string> linked_libraries "linked libraries"
    }
    
    %% ==========================================
    %% CONFIGURATION ENTITIES
    %% ==========================================
    
    CONFIG_SYSTEM {
        string FLEETCREDITS_CONF_FILENAME "fleetcredits.conf"
        string FLEETCREDITS_PID_FILENAME "fleetcreditsd.pid"
        string FLEETCREDITS_CONFIG_H "fleetcredits-config.h"
    }
    
    FLEETCREDITS_CONF_FILENAME {
        string value "fleetcredits.conf"
    }
    
    FLEETCREDITS_PID_FILENAME {
        string value "fleetcreditsd.pid"
    }
    
    FLEETCREDITS_CONFIG_H {
        string value "fleetcredits-config.h"
    }
    
    HelpMessageMode {
        uint8_t value "enum value"
    }
    
    HMM_FLEETCREDITSD {
        uint8_t value "daemon help mode"
    }
    
    HMM_FLEETCREDITS_QT {
        uint8_t value "Qt GUI help mode"
    }
    
    %% ==========================================
    %% SERIALIZATION ENTITIES
    %% ==========================================
    
    CDataStream {
        vector<unsigned char> vch "data buffer"
        uint32_t nReadPos "read position"
        uint32_t nVersion "version"
    }
    
    READWRITE {
        string macro_name "READWRITE macro"
    }
    
    ADD_SERIALIZE_METHODS {
        string macro_name "ADD_SERIALIZE_METHODS macro"
    }
    
    %% ==========================================
    %% TESTING ENTITIES
    %% ==========================================
    
    TestingSetup {
        CCHAINPARAMS chainParams "chain parameters"
        CWALLET testWallet "test wallet"
    }
    
    MWEBTest {
        CMWEBTRANSACTION testTx "test transaction"
        CMWEBEXTENSIONBLOCK testBlock "test extension block"
    }
    
    test_fleetcredits_qt {
        FleetCreditsGUI* gui "GUI instance"
        LIBFLEETCREDITS_QT qt_lib "Qt library"
    }
    
    %% ==========================================
    %% GOVERNANCE ENTITIES
    %% ==========================================
    
    CHALLENGE_TX {
        uint256 challenge_id PK "challenge ID"
        uint256 contrib_tx_id FK "contribution transaction ID"
        CFleetCreditsAddress challenger FK "challenger address"
        string challenge_reason "challenge reason"
        uint64_t challenge_timestamp "challenge timestamp"
    }
    
    VOTE_TX {
        uint256 vote_id PK "vote ID"
        uint256 proposal_id FK "proposal ID"
        CFleetCreditsAddress voter FK "voter address"
        uint8_t vote_choice "vote choice"
        int64_t vote_power "vote power"
        uint64_t vote_timestamp "vote timestamp"
    }
    
    BATCH_VERIFICATION {
        uint256 batch_id PK "batch ID"
        VALIDATORPOOL pool FK "validator pool"
        vector<uint256> contribution_ids "contribution IDs"
        uint64_t batch_timestamp "batch timestamp"
    }
    
    ANNOTATOR_VOTE {
        uint256 vote_id PK "vote ID"
        CPubKey annotator_pubkey FK "annotator public key"
        uint256 agreement_id FK "agreement ID"
        bool approval "approval vote"
    }
    
    GITHUB_COMMIT {
        uint256 commit_hash PK "commit hash"
        string repo_url "repository URL"
        uint32_t lines_changed "lines changed"
    }
    
    IPFS_HASH {
        uint256 hash PK "IPFS hash"
        string content_type "content type"
    }
    
    AI_OUTPUT_HASH {
        uint256 hash PK "AI output hash"
        string model_id "model ID"
        float accuracy_score "accuracy score"
    }
    
    DATASET_LABEL {
        uint256 label_id PK "label ID"
        string dataset_id "dataset ID"
        uint256 labels_hash "labels hash"
    }
    
    ETHICAL_RATIONALE {
        uint256 rationale_hash PK "rationale hash"
        string rationale_text "rationale text"
        uint64_t rationale_length "rationale length"
    }
    
    %% ==========================================
    %% HEADER GUARDS & INCLUDES
    %% ==========================================
    
    HEADER_GUARDS {
        string FLEETCREDITS_MODULE_NAME_H "naming convention"
        string FLEETCREDITS_SCRIPT_SCRIPT_H "script/script.h"
        string FLEETCREDITS_WALLET_WALLET_H "wallet/wallet.h"
        string FLEETCREDITS_INIT_H "init.h"
        string FLEETCREDITS_UTIL_H "util.h"
    }
    
    FLEETCREDITS_INCLUDES {
        string FLEETCREDITS_CONFIG_INCLUDES "config headers"
        string FLEETCREDITS_QT_INCLUDES "Qt headers"
    }
    
    FLEETCREDITS_MODULE_NAME_H {
        string pattern "FLEETCREDITS_MODULE_NAME_H"
    }
    
    FLEETCREDITS_SCRIPT_SCRIPT_H {
        string value "FLEETCREDITS_SCRIPT_SCRIPT_H"
    }
    
    FLEETCREDITS_WALLET_WALLET_H {
        string value "FLEETCREDITS_WALLET_WALLET_H"
    }
    
    FLEETCREDITS_INIT_H {
        string value "FLEETCREDITS_INIT_H"
    }
    
    FLEETCREDITS_UTIL_H {
        string value "FLEETCREDITS_UTIL_H"
    }
    
    FLEETCREDITS_CONFIG_INCLUDES {
        string value "config includes"
    }
    
    FLEETCREDITS_QT_INCLUDES {
        string value "Qt includes"
    }
    
    %% ==========================================
    %% CONSENSUS API ENTITIES
    %% ==========================================
    
    FLEETCREDITSCONSENSUS_API {
        string fleetcreditsconsensus_verify_script "verify script function"
        string fleetcreditsconsensus_verify_script_with_amount "verify with amount function"
        string fleetcreditsconsensus_version "version function"
        uint32_t FLEETCREDITSCONSENSUS_API_VER "API version = 1"
    }
    
    fleetcreditsconsensus_error {
        uint8_t value "error enum"
    }
    
    fleetcreditsconsensus_ERR_OK {
        uint8_t value "0"
    }
    
    fleetcreditsconsensus_ERR_TX_INDEX {
        uint8_t value "1"
    }
    
    fleetcreditsconsensus_ERR_TX_SIZE_MISMATCH {
        uint8_t value "2"
    }
    
    fleetcreditsconsensus_ERR_TX_DESERIALIZE {
        uint8_t value "3"
    }
    
    fleetcreditsconsensus_ERR_AMOUNT_REQUIRED {
        uint8_t value "4"
    }
    
    fleetcreditsconsensus_ERR_INVALID_FLAGS {
        uint8_t value "5"
    }
    
    %% ==========================================
    %% SIGNAL/SLOT SYSTEM ENTITIES
    %% ==========================================
    
    CClientUIInterface {
        ThreadSafeMessageBox messageBox "message box signal"
        ThreadSafeQuestion question "question signal"
        InitMessage initMessage "init message signal"
        ShowProgress showProgress "show progress signal"
        LoadWallet loadWallet "load wallet signal"
    }
    
    SafeBoolCombiner {
        boost::signals2::signal signal "boost signal"
        bool no_slots_error "no slots error prevention"
    }
    
    ThreadSafeMessageBox {
        string message "message text"
        string caption "caption text"
    }
    
    ThreadSafeQuestion {
        string question "question text"
        bool response "response boolean"
    }
    
    InitMessage {
        string message "init message text"
    }
    
    ShowProgress {
        string title "progress title"
        int nProgress "progress percentage"
    }
    
    LoadWallet {
        string walletFile "wallet file path"
    }
    
    %% ==========================================
    %% ADDITIONAL ENTITY ATTRIBUTES (completeness)
    %% ==========================================
    
    CKeyStore {
        map<CKeyID, CKey> mapKeys "keys map"
        map<CScriptID, CScript> mapScripts "scripts map"
    }
    
    CMerkleTx {
        uint256 hashBlock "block hash"
        vector<uint256> vMerkleBranch "merkle branch"
        int nIndex "transaction index"
    }
    
    CAccountingEntry {
        string strAccount "account name"
        CAmount nCreditDebit "credit/debit amount"
        int64_t nTime "entry time"
        string strOtherAccount "other account"
        string strComment "comment"
    }
    
    CKeyMetadata {
        int64_t nCreateTime "creation time"
    }
    
    CAddressBookData {
        string name "address name"
        string purpose "address purpose"
    }
    
    CKeyPool {
        int64_t nTime "key creation time"
        CPubKey vchPubKey "public key"
    }
    
    CReserveKey {
        CWallet* pwallet "parent wallet"
        int64_t nIndex "key pool index"
        CPubKey vchPubKey "reserved public key"
    }
    
    CRecipient {
        CScript scriptPubKey "output script"
        CAmount nAmount "amount"
        bool fSubtractFeeFromAmount "subtract fee flag"
    }
    
    WalletFrame {
        WalletView* walletView "wallet view"
        WalletModel* walletModel "wallet model"
    }
    
    WalletView {
        OverviewPage* overviewPage "overview page"
        SendCoinsDialog* sendCoinsDialog "send coins dialog"
        ReceiveCoinsDialog* receiveCoinsDialog "receive coins dialog"
        TransactionView* transactionView "transaction view"
    }
    
    OverviewPage {
        CAmount currentBalance "current balance"
        CAmount currentUnconfirmedBalance "unconfirmed balance"
        CAmount currentImmatureBalance "immature balance"
    }
    
    SendCoinsDialog {
        WalletModel* walletModel "wallet model"
        vector<SendCoinsRecipient> recipients "recipients"
    }
    
    ReceiveCoinsDialog {
        WalletModel* walletModel "wallet model"
        QLabel* labelAddress "address label"
        QLabel* labelAmount "amount label"
    }
    
    TransactionView {
        TransactionTableModel* model "transaction model"
        QTreeView* view "tree view"
    }
    
    TransactionRecord {
        uint256 hash "transaction hash"
        int64_t time "transaction time"
        TransactionRecord::Type type "record type"
        CAmount credit "credit amount"
        CAmount debit "debit amount"
    }
    
    RecentRequestsTableModel {
        vector<RecentRequestEntry> entries "recent request entries"
    }
    
    OptionsModel {
        bool fMinimizeToTray "minimize to tray"
        bool fMinimizeOnClose "minimize on close"
        bool fUseFleetCreditsAddresses "use Fleet Credits addresses"
    }
    
    PlatformStyle {
        string name "platform style name"
        QIcon icon "icon"
        QColor singleColor "single color"
    }
    
    NetworkStyle {
        string appName "application name"
        QIcon appIcon "application icon"
        QPixmap splashImage "splash image"
    }
    
    SendCoinsRecipient {
        CFleetCreditsAddress address "recipient address"
        CAmount amount "amount"
        string label "label"
        string message "message"
    }
    
    RecentRequestEntry {
        CFleetCreditsAddress address "address"
        CAmount amount "amount"
        string label "label"
        int64_t nTime "time"
    }
    
    TransactionFilterProxy {
        int dateFrom "date from"
        int dateTo "date to"
        TransactionRecord::Type type "type filter"
        CAmount minAmount "minimum amount"
    }
    
    CInPoint {
        CTransactionRef ptx "transaction reference"
        uint32_t n "output index"
    }
    
    CDiskBlockPos {
        int nFile "file number"
        unsigned int nPos "byte position"
    }
    
    arith_uint256 {
        uint256 value "256-bit value"
    }
    
    CTRANSACTIONREF {
        CTransactionRef ptr "transaction reference pointer"
    }
    
    mapValue_t {
        map<string, string> value "metadata map"
    }
    
    TxSpends {
        multimap<COutPoint, uint256> spends "spends map"
    }
    
    CHDChain {
        int nVersion "HD chain version"
        uint256 seedID "seed ID"
        int nExternalChainCounter "external chain counter"
        int nInternalChainCounter "internal chain counter"
    }
    
    CMasterKey {
        vector<unsigned char> vchCryptedKey "encrypted key"
        vector<unsigned char> vchSalt "salt"
        unsigned int nDeriveIterations "derive iterations"
        unsigned int nDeriveMethod "derive method"
        vector<unsigned char> vchOtherDeriveParameters "other derive parameters"
    }
    
    WalletFeature {
        uint8_t value "feature enum"
    }
    
    FEATURE_BASE {
        uint8_t value "base feature"
    }
    
    CAccountingEntry {
        string strAccount "account name"
        CAmount nCreditDebit "credit/debit amount"
        int64_t nTime "entry time"
        string strOtherAccount "other account"
        string strComment "comment"
    }
    
    CTxDestination {
        CKeyID keyID "key ID"
        CScriptID scriptID "script ID"
    }
    
    CNoDestination {
        bool isNull "null destination flag"
    }
    
    CKeyID {
        uint160 value "160-bit key ID"
    }
    
    CScriptID {
        uint160 value "160-bit script ID"
    }
    
    uint160 {
        unsigned char bytes "20 bytes"
    }
    
    uint256 {
        unsigned char bytes PK "32 bytes"
    }
    
    DBErrors {
        uint8_t value "database error enum"
    }
    
    FeeRatePreset {
        uint8_t value "fee rate preset enum"
    }
    
    BlockStatus {
        uint32_t value "block status flags"
    }
    
    BLOCK_VALID_HEADER {
        uint32_t value "valid header flag"
    }
    
    BLOCK_VALID_TREE {
        uint32_t value "valid tree flag"
    }
    
    BLOCK_VALID_TRANSACTIONS {
        uint32_t value "valid transactions flag"
    }
    
    BLOCK_VALID_CHAIN {
        uint32_t value "valid chain flag"
    }
    
    BLOCK_VALID_SCRIPTS {
        uint32_t value "valid scripts flag"
    }
    
    BLOCK_HAVE_DATA {
        uint32_t value "have data flag"
    }
    
    BLOCK_HAVE_UNDO {
        uint32_t value "have undo flag"
    }
    
    BLOCK_FAILED_MASK {
        uint32_t value "failed mask"
    }
    
    BLOCK_VALID_MASK {
        uint32_t value "valid mask"
    }
    
    COIN {
        CAmount value "100000000 koinu"
    }
    
    MAX_MONEY {
        CAmount value "maximum money amount"
    }
```

## Key Implementation Notes

### Build System Architecture
- **Libraries**: All libraries use `LIBFLEETCREDITS_*` naming (no legacy `LIBDOGECOIN_*` or `LIBBITCOIN_*`)
- **Executables**: `fleetcreditsd`, `fleetcredits-qt`, `fleetcredits-cli`, `fleetcredits-tx`
- **Consensus Library**: `libfleetcreditsconsensus.la` - shared library for external use
- **Qt Components**: `LIBFLEETCREDITS_QT` links to Qt GUI applications
- **Includes**: `FLEETCREDITS_INCLUDES`, `FLEETCREDITS_QT_INCLUDES`, `FLEETCREDITS_CONFIG_INCLUDES`

### Contribution Types (8 Total)
1. **CODE_CONTRIBUTION** (0x01): Open-source code commits
2. **CHARITABLE_ACT** (0x02): Verified charitable donations
3. **CREATIVE_WORK** (0x03): Art, writing, music with IPFS hash
4. **EDUCATIONAL_CONTENT** (0x04): Teaching materials or courses
5. **CODE_MENTORSHIP** (0x05): Mentor-mentee code reviews
6. **AI_VALIDATION** (0x06): Human validation of AI outputs
7. **DATA_LABELING** (0x07): Dataset labeling with inter-annotator agreement
8. **ETHICAL_REVIEW** (0x08): Mandatory MWEB for privacy

### MWEB Integration
- **Mandatory** for `ETHICAL_REVIEW` contributions
- **Optional** for standard transactions
- Uses **Pedersen commitments** for amount hiding
- **Range proofs** ensure non-negative values
- **Cut-through** reduces blockchain bloat
- **Peg-in/Peg-out** for atomic swaps between main chain and MWEB

### Fee Structure
- **Zero fees** for transactions < 1000 FC (`MICRO_TX_THRESHOLD`)
- **Scalable fees** for transactions >= 1000 FC
- **1% of fees** allocated to Community Reserve
- Reserve managed by **multisig wallet**

### Block Reward System
- **Base reward**: 10,000 FC (fixed, no halving)
- **Bonus multipliers**:
  - BONUS_NONE: 1.0x
  - BONUS_LOW: 1.05x
  - BONUS_MEDIUM: 1.10x
  - BONUS_HIGH: 1.15x
  - BONUS_CRITICAL: 1.20x (1.25x for AI_VALIDATION)

### Verification System
- **Oracles**: Minimum 500k FC stake, 3-of-5 consensus
- **Human Validators**: Reputation-gated pools for batch verification
- **Slashing**: 50% stake loss for bad oracle votes
- **Inter-annotator agreement**: >= 0.7 for data labeling
- **Ethical Evaluation**: Two-layer framework (objective criteria + oracle value judgment)
  - **Objective Criteria**: Hard requirements (valid proof, non-duplicate, minimum thresholds)
  - **Value Judgment**: Oracle consensus on impact, authenticity, alignment, harm prevention
  - **Documentation**: Ethical guidelines documented in `contribution-ethics.html`
  - **Implementation**: Framework exists in codebase; specific criteria enforced by oracle judgment

### Network Parameters
- **Mainnet**: P2P port 22556, RPC port 22555
- **Testnet**: P2P port 44556, RPC port 44555
- **Regtest**: P2P port 18444, RPC port 18332
- **Message start bytes**: `0xfc, 0xc1, 0xb7, 0xdc`
- **Block time**: 60 seconds
- **Genesis timestamp**: January 1, 2026 00:00:00 UTC (1704067200)
- **Genesis block**: Mined with valid PoW at initialization
- **Genesis PoW**: Validated via `CheckProofOfWork` using Scrypt hash
- **Regtest difficulty**: `0x207fffff` (very low, allows instant mining)
- **Genesis validation**: PoW checked during `AcceptBlock`, checkpoint validated during `ReadBlockFromDisk`

### RPC Commands
- **Contribution**: `submitcontribution`, `getcontributionstatus`, `listcontributions`
- **MWEB**: `createpegin`, `createpegout`, `routecontributiontomweb`, `listmwebtxs`
- **Mining**: `generatetoaddress`, `getnewaddress`, `getblockcount`, `getbalance`
- **Blockchain**: Standard blockchain RPC commands (getblock, gettxout, etc.)
- **Network**: Standard network RPC commands (getnetworkinfo, etc.)
- **Raw Transaction**: Standard raw transaction commands

### Code Naming Conventions
- **Header Guards**: `FLEETCREDITS_MODULE_NAME_H` (e.g., `FLEETCREDITS_SCRIPT_SCRIPT_H`, `FLEETCREDITS_WALLET_WALLET_H`)
- **Classes**: All classes prefixed with `C` (CBlock, CTransaction, etc.)
- **Fleet Credits Types**: `CFleetCreditsAddress`, `CFleetCreditsSecret`, `CFleetCreditsExtKey`, etc.
- **MWEB Types**: `CMWEBTransaction`, `CPedersenCommitment`, `CMWEBExtensionBlock`, etc.
- **Contribution Types**: `CContributionTransaction`, `CMentorshipTransaction`, `CAIValidationTransaction`, etc.
- **Verification Types**: `COracleNode`, `CVerificationRecord`, `CContributionVerifier`, etc.
- **Qt Classes**: `FleetCreditsGUI`, `FleetCreditsUnits`, `FleetCreditsAddressValidator`, `FleetCreditsAmountField`, etc.
- **Enums**: `HelpMessageMode` with values `HMM_FLEETCREDITSD`, `HMM_FLEETCREDITS_QT`
- **Constants**: `FLEETCREDITS_CONF_FILENAME`, `FLEETCREDITS_PID_FILENAME`
- **Build Variables**: `FLEETCREDITS_INCLUDES`, `FLEETCREDITS_QT_INCLUDES`, `FLEETCREDITS_QT_CPP`, `FLEETCREDITS_QT_H`, etc.

### Consensus Library API
- **Header**: `script/fleetcreditsconsensus.h` (removed legacy `dogecoinconsensus.h`)
- **Implementation**: `script/fleetcreditsconsensus.cpp`
- **Functions**: 
  - `fleetcreditsconsensus_verify_script()` - Verify script without amount
  - `fleetcreditsconsensus_verify_script_with_amount()` - Verify script with amount (required for Segwit)
  - `fleetcreditsconsensus_version()` - Returns API version (currently 1)
- **Error Types**: `fleetcreditsconsensus_error` enum with values:
  - `fleetcreditsconsensus_ERR_OK` (0)
  - `fleetcreditsconsensus_ERR_TX_INDEX` (1)
  - `fleetcreditsconsensus_ERR_TX_SIZE_MISMATCH` (2)
  - `fleetcreditsconsensus_ERR_TX_DESERIALIZE` (3)
  - `fleetcreditsconsensus_ERR_AMOUNT_REQUIRED` (4)
  - `fleetcreditsconsensus_ERR_INVALID_FLAGS` (5)
- **No Legacy Aliases**: All dogecoin/bitcoin consensus references removed

### Genesis Block Mining Process
1. **Creation**: `CreateGenesisBlock()` creates genesis block with initial parameters
2. **Mining**: `chainparams.cpp` mines genesis block by incrementing `nNonce` until `CheckProofOfWork()` returns true
3. **Validation**: Genesis block PoW validated during `AcceptBlock()` in `validation.cpp`
4. **Checkpoint**: Genesis hash stored in `CCheckpointData` at height 0
5. **Consistency**: `ReadBlockFromDisk()` validates genesis block against `consensusParams.hashGenesisBlock`
6. **Robustness**: Genesis block always has valid PoW - no skipping or bypassing validation

### Validation Flow
- **Genesis Block**: PoW validated during `AcceptBlock()` → hash stored in consensus params
- **Subsequent Blocks**: PoW validated via `CheckAuxPowProofOfWork()` in `ReadBlockFromDisk()` and `AcceptBlock()`
- **Checkpoints**: Genesis block hash validated against checkpoint data
- **Consistency**: All blocks, including genesis, must pass PoW validation

### Contribution Reward Distribution Flow
1. **Contribution Submission**: 
   - User calls `submitcontribution` RPC with proof data
   - `CContributionTransaction` created with contributor's public key
   - Transaction serialized and embedded in `OP_RETURN` with marker `0xFC 0x01`
   - Transaction signed and broadcast to network

2. **Block Mining**:
   - Miner includes contribution transaction in block
   - `ExtractContributionsFromBlock()` parses all transactions
   - Finds `OP_RETURN` outputs with contribution marker
   - Deserializes `CContributionTransaction` data
   - Validates using `ValidateContributionTransaction()`

3. **Reward Calculation**:
   - `GetFleetCreditsBlockSubsidyWithContributions()` called during `ConnectBlock()`
   - Base reward: 10,000 FC (from `GetFleetCreditsBlockSubsidy()`)
   - Highest bonus level found across all contributions in block
   - `GetBonusMultiplier()` calculates multiplier (1.0x to 1.25x)
   - Final reward = base reward × multiplier

4. **Reward Distribution**:
   - Block reward added to coinbase transaction (first transaction in block)
   - **Note**: In current implementation, rewards are paid to miner's coinbase
   - **Future Enhancement**: Direct contributor payouts via separate outputs
   - Contributors can verify rewards via `getcontributionstatus` RPC

5. **MWEB Contributions**:
   - Contributions with `requires_mweb=true` are routed to MWEB extension block
   - `MWEB::RouteToMWEB()` converts to `CMWEBTransaction`
   - Extracted via `ExtractContributionsFromMWEB()` during block validation
   - Rewards calculated separately for MWEB contributions
   - Privacy preserved via Pedersen commitments

### Qt GUI Architecture
- **Main Window**: `FleetCreditsGUI` - primary application window
- **Models**: `ClientModel`, `WalletModel`, `OptionsModel` - data models for MVC architecture
- **View Components**: `WalletView`, `OverviewPage`, `SendCoinsDialog`, `ReceiveCoinsDialog`
- **Table Models**: `TransactionTableModel`, `AddressTableModel`, `RecentRequestsTableModel`
- **Utilities**: `FleetCreditsUnits` (formatting), `FleetCreditsAddressValidator`, `FleetCreditsAmountField`
- **Theme System**: `ThemeManager` for UI customization

### File Structure Logic
1. **Source Organization**: Logical module separation (wallet/, rpc/, qt/, script/, primitives/)
2. **Build Libraries**: Clear separation between server, common, consensus, CLI, util, crypto, Qt, wallet
3. **Header Guards**: Consistent `FLEETCREDITS_MODULE_NAME_H` pattern
4. **Includes**: Centralized via `FLEETCREDITS_INCLUDES` and module-specific includes
5. **Naming**: All Fleet Credits-specific types use `FleetCredits` or `FLEETCREDITS` prefix

### Logical Flow Verification (Validated Against Implementation)

**All flows verified against actual codebase:**

1. **Transaction Submission Flow** ✓
   - `submitcontribution` RPC (rpc/contribution.cpp:112)
   - Creates `CContributionTransaction` with proof data
   - Serializes with `CDataStream` and `PROTOCOL_VERSION`
   - Embeds in `OP_RETURN` with marker `0xFC 0x01` (2 bytes)
   - Signs transaction using wallet (`pwalletMain->CommitTransaction`)
   - Broadcasts via network (`g_connman->RelayTransaction`)

2. **Block Processing Flow** ✓
   - `ExtractContributionsFromBlock()` (fleetcredits.cpp:197)
   - Scans block transactions (skips coinbase, index 0)
   - Finds `OP_RETURN` outputs with contribution marker
   - Deserializes `CContributionTransaction` from script data
   - Validates using `ValidateContributionTransaction()`
   - Returns vector of valid contributions

3. **Reward Calculation Flow** ✓
   - Called during `ConnectBlock()` (validation.cpp:2012)
   - `GetFleetCreditsBlockSubsidy()` gets base 10,000 FC reward
   - `GetFleetCreditsBlockSubsidyWithContributions()` finds max bonus level
   - `GetBonusMultiplier()` (contribution.cpp:14) calculates multiplier
   - Formula: `baseReward × multiplier` (e.g., 10,000 × 1.25 = 12,500 FC)

4. **Verification Flow** ✓
   - `CContributionVerifier` class exists (primitives/verification.h:231)
   - `CreateVerificationRecord()` creates record for contribution
   - `SelectRandomOracles()` selects 3-5 oracles with stake >= 500k FC
   - `ProcessOracleVotes()` aggregates votes
   - 3-of-5 consensus required for approval
   - Status stored in `CVerificationRecord`

5. **MWEB Routing Flow** ✓
   - `requires_mweb = true` checked in `RequiresMWEB()` (contribution.h:113)
   - `MWEB::RouteToMWEB()` (mweb.cpp:211) converts contribution
   - Creates `CMWEBTransaction` with Pedersen commitments
   - Generates range proofs
   - Pegs to extension block during block creation
   - `ExtractContributionsFromMWEB()` extracts during validation (validation.cpp:2028)

6. **Reward Distribution Flow** ✓
   - Block reward calculated and added to coinbase transaction
   - Coinbase is first transaction in block (vtx[0])
   - Miner receives reward in coinbase output
   - **Current Implementation**: Rewards go to miner (who includes contributions)
   - **Future Enhancement**: Direct contributor payouts possible via additional coinbase outputs

**All flows are logically consistent and match implementation.**

---

**Generated**: After complete codebase conversion and naming convention cleanup  
**Last Updated**: Post-complete cleanup (removed all legacy aliases, updated all naming conventions)  
**Alignment**: This ERD reflects the actual codebase structure, including:
- All build system libraries and executables
- Complete naming convention alignment (FLEETCREDITS_*, FleetCredits*, etc.)
- Removed legacy consensus files (dogecoinconsensus.h/cpp)
- Updated header guards (184 files)
- Updated Qt components and models
- Configuration constants and help message modes
- Consensus library API structure
