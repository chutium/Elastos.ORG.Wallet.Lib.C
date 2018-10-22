
#ifndef __TRANSACTION_H__
#define __TRANSACTION_H__

#include "UTXOInput.h"
#include "TxOutput.h"
#include "Attribute.h"
#include "Program.h"
#include "../BRInt.h"
#include "../ByteStream.h"
#include <vector>
#include "nlohmann/json.hpp"

#define TX_LOCKTIME          0x00000000

class Transaction
{
    enum Type {
        CoinBase                = 0x00,
        RegisterAsset           = 0x01,
        TransferAsset           = 0x02,
        Record                  = 0x03,
        Deploy                  = 0x04,
        SideMining              = 0x05,
        IssueToken              = 0x06,
        WithdrawAsset           = 0x07,
        TransferCrossChainAsset = 0x08,
        RegisterIdentification  = 0x09,
        TypeMaxCount
    };

public:
    Transaction();

    Transaction(std::vector<UTXOInput*> inputs, std::vector<TxOutput*> outputs, std::string memo);

    ~Transaction();

    UInt256 GetHash();

    void Serialize(ByteStream &ostream);

    void Sign(const CMBlock& privateKey);

    void MultiSign(const CMBlock& privateKey, const CMBlock& redeemScript);

    std::vector<CMBlock> GetPrivateKeys();

    void FromJson(const nlohmann::json &jsonData);

    nlohmann::json ToJson();

private:
    void SerializeUnsigned(ByteStream &ostream) const;

    CMBlock SignData(const CMBlock& privateKey);

private:
    Type mType;
    uint8_t mPayloadVersion;
    void* mPayload;

    std::vector<UTXOInput*> mInputs;
    std::vector<TxOutput*> mOutputs;
    std::vector<Attribute*> mAttributes;
    std::vector<Program*> mPrograms;

    uint32_t mLockTime;

    uint64_t mFee;
    UInt256 mTxHash;
};

#endif //__TRANSACTION_H__
