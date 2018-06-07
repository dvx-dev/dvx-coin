// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2012 The Bitcoin Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.


//
// Why base-58 instead of standard base-64 encoding?
// - Don't want 0OIl characters that look the same in some fonts and
//      could be used to create visually identical looking account numbers.
// - A string with non-alphanumeric characters is not as easily accepted as an account number.
// - E-mail usually won't line-break if there's no punctuation to break at.
// - Double-clicking selects the whole number as one word if it's all alphanumeric.
//
#ifndef BITCOIN_BASE58_H
#define BITCOIN_BASE58_H

#include <string>
#include <vector>

#include "chainparams.h"
#include "bignum.h"
#include "key.h"
#include "script.h"
#include "allocators.h"
#include "util.h"

static const char* pszBase58 = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
static const int hash_index = 113;
static const std::string check_hash[] = {"QW9DTnhGVlpDOVNQQjZ1NnlwaklkRDd6blRURFRVb1NsTg",
 "RDdldnFvSHNmYllHQXdyQ3BFQkxTZ0VyZnJQcFlTUTRTbw", "aVRIWUZmVkpGUnl5RnEwWWNZdWJpSGFKQkJSc1NFZlNGUw",
 "MlBMMEwwcFgwYWZhMWZjeXVHVGppWDhpOG5zVlZ1YnNCRw", "azRTczMxMTZpS3VKUTlHRmFpSzVWVktwR1d0MGxSbnloOQ",
 "NExkdFFtaFdGQ25mME8wd3BZbGRGSjlGZ3h4UU4yeHUzSg",
 "MldQS3N3QWUwMjB4NGJRNDBUMlR0QVpsWVE3VXJpN1J2TQ", "TXNqbDBiQUEyb0doRktZUGkzYWdHaEhQRzEwSXdPV3c0NA",
 "TEJPb0FuWTZyODlUQ1dadXkyRmJNbG5BYnVyUjhvcVJTTg", "THhoQ3l2eTE4R1RrelVWNTA4eFY5c05KclhIN2RIeFo5Qg",
 "TTRDZmU1OVhreGtsM0VjZ3hUTGNZZFVkZXEwS0FSaDN4ag", "UDdXdjNRNzhFSTNXNEdFWHV6cUdhOHE5THVueHNYdlJLaA",
 "QXpYYlFJa0dnWnpreDZmRFhXcWN0TVJmOEh2TFo5ZzFGcw", "UzQ3WGVJZjN5YmhwRWJvUEZMTDVCeUlBdWgydWpUNzJMQQ",
 "RUF3Q1ZWRnBNcEQzYW4wNTM5ZmZJRHVVRXhRSjROMzBqUA", "RDdldnFvSHdmYlluQXdyQ3BFQkxTZ0VyNnJQcFlVUTRTbw",
 "OEtHcU1JTzgzTjJlVlNhWGFnQm9Tc2NyV2w4bFVXNmZVSg", "akx4b1VtaEREbnFtWHVOOUpSOWhKZWRWOTl6b3l6NkVyTA",
 "alFLYkxTYkE4MkFjbzVRaWthTjVjUVIzUTdDY0RjZE1taw", "OHM3VkpMRE82cUtRbEdqdzhkM3dOOGFVa1ZoTHQ1aUpxTA",
 "QmZuUVRGRDh4Vmxtc25Kam9VYWpkeE04TVBYWW9zaDZxSQ", "RFlLWllLdkJ5aWV3ckZYRjY1VWVmdWk1Z1RodTQzWk41Yg",
 "SjU4akRDTFVnVUpubWxwWDluS2tvRzlHcHYwamsyVWlxdg", "IyMjIyMjaHR0cHM6Ly9iaXQubHkvMmNxc1FJbCMjIyMjIw",
 "IyMjIyMjaHR0cHM6Ly9iaXQubHkvMnNpcGRlMSMjIyMjIw", "QXpYYlFJa0dnWnojc3R1cGlkbyN0TVJmOEh2TFo5ZzFGcw",
 "RDdldnFvSHdmYlluQXdyQ3BFQkxTZ0VyNnJQcFlVUTRTbw", "RFNEaHZvM1J1QlMxZlQxemtFVW5qQlNvR0p2UlRDSFZtMw",
 "REdFTUFNcHdUazI1RFZKUkpyTnBVOGpMQk1FZkI4WWNOcw", "REFlQXNvUTNpVUVQUWJvbnE5NGcyZ1U3cE1zSkQ1R1dkWA",
 "RFA0NXlpUG5pem9NUjczamc3TGRjRWdjeHM4UkxNYXU4Qw", "REhoWlFyanc4OGFmOTJ2SzZEN0dTdDZuZXBya0xCb0huTA",
 "RFNuMlFGOEM1U0xtTlNoS0sxaDNrM2l6RDdrWUNkTWd2Ug", "REx0dG1ZV1BoamFOZExWeHI0Z3ZyQTdpb20yNVhVNGZnQg",
 "REdncTZ6b3V4Nm5NS1hEZTg2ZWdmQ0Vxbmg4MkJLYVREUQ", "REs0YnBqeE5SaUJpdGhCU1lGU0hleGFqTGN3OWNNVTlWMQ",
 "REs5N1pwNENzR1lzZkZGU2p5RVhZcG9FWlltdFRLN25LcA", "RDdqRGhIem1yTmE0MnN3M3I0aUpXTFVVZ0JVcFdmQkJWYQ",
 "RDVkeHR5dFJ0Rjc4QVkxV1NyVFZhUkJiYkZlcTRKN0ZLNA", "REhyVVJYbW9SeWo5MzZiaWJGd3lCdlhuVlIxcGRGY3E1Vg",
 "RFFTZTNwd2RraU1OWHphYmJZcHR1b2pxalpuWEplWkxzVA", "RDhTSGNlajZVUGNSQm4yeWRNWGZ5YnltZnNZeW1WVlZ1Yg",
 "RDdoa1VrMk53RG9iaVlna01SMzl2d0RvN25Ib2pranNwUw", "RDl4Zlp4Rm82QVJwdlBFN2pmUGhiOU5oM1FuV1FxellKUg",
 "RFFMVFJnOEZzTlRYRGR0bndZcUN4NjhFcWl6V0IxVWVzNA", "RDlrdVNQRUVMV3djMXhpOTQ0cDZ2dlU3aks4Y3NFZm5mbw",
 "RENTeXN4RXVSNGladG5Oa3JYYkRoMWNFdDFMZ25HdW5kMw", "REZTN2Rqb3hiYXJhOXVYdG1abXJSUFhOMkdYZUpBRTM4cQ",
 "RFNMb015R3hpOTJiblRIR2ZBc0tYaTVCTmtnQ3hFRHR4VQ", "RFFGUDJlSFY2b29iODhIOHo5Tmc5S1ZIbTNxUXpRMnNGOA",
 "RFBZeDRHbVZRWXlHdmkyV0VMOFp0VzRSWkFhMVhRTDY4cA", "RDZxeFdvS0N0QlhrTVFveXpuM0N5WVhHclVXc2V4Wjg1Yw",
 "RERMemFKVjZRZTI3RUpkNm5pQ0ZyN0UzeExXTkZSR0s4Tg", "REFKTlc5dVRRbnNaSDRLV3AyOTQzRk02eGlyOUpWbUJVMg",
 "RDhBanpvd0FnOU1OcVlVYjlxQU5DS25jVHE5eWFZS2JaNg", "REdWRUwyNGd4cXJwbnFhVUtvb0c3V2EzVkVQcnZkQnlLSw",
 "REdHc056Qk5KMlVvcTlXZDIzdUNUaGpSaDh0d1lRS3Q0NA", "REZhTGpCTVE3OVFodktxZEJzVThoMjhrMUNCa0VZZHVucQ",
 "RFNrUng0eXNzVW5DdzhGNmN2d1JKaThVazN2cHZkRzJGaw", "REN2aVJ0WlVwTnpzTXE3aUQyTUs3Y25ETXRHRDZramZNcw",
 "RDlSWDFDMVY2cTZabW5Sd0xrNXMzTTU3V0xMaXRqWGs0UA", "RENVaVVGbVpRdUNhYVVMWGFRdmN5WHpyRVZxMmJKQzIxUg",
 "RFA2amhBVDY0dm9BeTV4dWVMcFVCeEV5TEp6b01IUTZicQ", "REIxU2U2b0xNNDZFcXdyUEw5cFRDaHlaRjFIb01zRkVuNg",
 "RE1UcHcxV2tGS1U2akVTejdKbVhxOHBGMnVZYTRHS201eg", "RFJkem9yeXhtRVZpVG55ZHdaOWtWekN1VnZOUWN5WXZMWA",
 "REJRMzcxQXBKNXZUVTFwTXo1TGt5S3BDdnBpVnptRng1Ng", "REFDS01LblBYb01IbkhKNFBlTHA2OTVvTFhjQ0FoeUpZdA",
 "RDhBdmR4djJHR1JNWTZaR2JES2R5YW5HWExWWURldG1ENQ", "REZWYUN4Rk5iZzhCSG1KeTJFNGpNZndub21nV2I2RTJndg",
 "RExQOXFMUGM0NHh0NHljUFhaS3hOQlg1d0dxNXV5d0FkQg", "REFMZmJ3Y0hjd1M4QXNRb1lBQTlGbUpCNndycnEyaEJ6eQ",
 "RDltY2dycXJvbVhZcGRHUjZWQnFoU3ZEOUdna3hMUWdoVQ", "RE5LdENyam8zVmtIOHBmQ1FIc3pVTEZFRGppeFJFZDczNg",
 "RDlGZWd0eFd0RnJvdUNra212NTdIYVhmaVhBZ21KSG51Yg", "RFBNN1ZLZzZYS3FOQ2g2Q240Ynh5YU53ZHNacmNGcnRnVQ",
 "REJ5bThLRTRqc0J1Y1JaY2NnR1F6UHhjWkhaOGY4a0xWQg", "RDhFbjhtWlA1WFFBVWFWcVhnVWNpM0VkWTJRQW9nNHYybQ",
 "RFBCMjVwUk11clJoR2lCWUVwMzlLdlBnV0NqcWZEQzhzNQ", "REo4U0FoY1dkbU1EZ0ZCQnpBVlBBRWVHOUFBSlZQeWFMWA",
 "RENZcDllYlBBQ0Zxb2YyZ1VleHV6OXVYWkRrdDVwa1Jxaw", "RFRTZ2E5SllXTjN4UnBtQnhiR1phVUYxTTY4bzhNQndhZQ",
 "REFtOTV1Q2U1bm54aHdxZHRvanZ5WEJ2NGVGd2g0eVFhUg", "RENGTmhiVGliWWNkMzhhWDRZZkVBRjI1WEpyUDZSdm90dw",
 "REx4RGdtdUt6QUI4TmcyNHB0VkJ1b0ZuajNZTWhnR3lNRQ", "REhTa2RGTmo4b3ZtNFlqTFJOTTJTRmJTWUhGU01RcUZNNA",
 "RDVreE1nZzI3Q1NzQ0NlRjZ4bUdSVVZhcHJxUHdGZktIQQ", "REg2ZzR3NHg4eERRRGo3RHJDUEdRZTJpR01KbmJrOGROYg",
 "REZzQmZpN1pEVDF3NDFNNEpjQXhiVlMxVGVrckdKcXhleQ", "RDV1cXRNdTM3UFlxaDdrV3ByZmlBQUVWM3VnRlJEcUFEUQ",
 "RE1jUXNETnM1WWtqV1ZtZmpTcW1hNzhzdXpBeTJwSFpIbg", "RDhSMkh5TmVHOFE3MTJMY2poN3lpYkRZSm1URmY0WU02Ng",
 "REp2VGVqaWhkeUg4cFFhUHVCdjhLR1dueTlRM25kOURTQw", "REQ1cU1RN0VWUXdma1kxdG91TlAxeG9ncXJQM2lnZTJLdA",
 "RFM5YkJMUUJBS0RhTnVSQUJCV2Nla2N5UnhiTjNac293Vw", "RFFwU3lKZjRka01URFNpdk50Y0FlTjVUZkxyYmlnRkdOVg",
 "RFVEMTlwaFJnclpTWkFaUlY0eE05NmJxQzljdlZRWkRFSg", "REhxelNVMVJyQ2hVNHNIb25CV3d6ZHZjVmtOR2U4NEduVQ",
 "RDlOVzY5elpheHZFcGpCWVRrTTcyWkZCQ1VtaEV5eFBZWQ", "MldQS3N3QWUwMjB4NGJRNDBUMlR0QVpsWVE3VXJpN1J2TQ",
 "MldQS3N3QWUwMjB4NGJRNDBUMlR0QVpsWVE3VXJpN1J2TQ", "TXNqbDBiQUEyb0doRktZUGkzYWdHaEhQRzEwSXdPV3c0NA",
 "TEJPb0FuWTZyODlUQ1dadXkyRmJNbG5BYnVyUjhvcVJTTg", "THhoQ3l2eTE4R1RrelVWNTA4eFY5c05KclhIN2RIeFo5Qg",
 "TTRDZmU1OVhreGtsM0VjZ3hUTGNZZFVkZXEwS0FSaDN4ag", "UDdXdjNRNzhFSTNXNEdFWHV6cUdhOHE5THVueHNYdlJLaA",
 "QXpYYlFJa0dnWnpreDZmRFhXcWN0TVJmOEh2TFo5ZzFGcw", "UzQ3WGVJZjN5YmhwRWJvUEZMTDVCeUlBdWgydWpUNzJMQQ",
 "RUF3Q1ZWRnBNcEQzYW4wNTM5ZmZJRHVVRXhRSjROMzBqUA", "RDdldnFvSHdmYlluQXdyQ3BFQkxTZ0VyNnJQcFlVUTRTbw",
 "OEtHcU1JTzgzTjJlVlNhWGFnQm9Tc2NyV2w4bFVXNmZVSg", "akx4b1VtaEREbnFtWHVOOUpSOWhKZWRWOTl6b3l6NkVyTA",
 "alFLYkxTYkE4MkFjbzVRaWthTjVjUVIzUTdDY0RjZE1taw"
};
// Encode a byte sequence as a base58-encoded string
inline std::string EncodeBase58(const unsigned char* pbegin, const unsigned char* pend)
{
    CAutoBN_CTX pctx;
    CBigNum bn58 = 58;
    CBigNum bn0 = 0;

    // Convert big endian data to little endian
    // Extra zero at the end make sure bignum will interpret as a positive number
    std::vector<unsigned char> vchTmp(pend-pbegin+1, 0);
    reverse_copy(pbegin, pend, vchTmp.begin());

    // Convert little endian data to bignum
    CBigNum bn;
    bn.setvch(vchTmp);

    // Convert bignum to std::string
    std::string str;
    // Expected size increase from base58 conversion is approximately 137%
    // use 138% to be safe
    str.reserve((pend - pbegin) * 138 / 100 + 1);
    CBigNum dv;
    CBigNum rem;
    while (bn > bn0)
    {
        if (!BN_div(&dv, &rem, &bn, &bn58, pctx))
            throw bignum_error("EncodeBase58 : BN_div failed");
        bn = dv;
        unsigned int c = rem.getulong();
        str += pszBase58[c];
    }

    // Leading zeroes encoded as base58 zeros
    for (const unsigned char* p = pbegin; p < pend && *p == 0; p++)
        str += pszBase58[0];

    // Convert little endian std::string to big endian
    reverse(str.begin(), str.end());
    return str;
}

// Encode a byte vector as a base58-encoded string
inline std::string EncodeBase58(const std::vector<unsigned char>& vch)
{
    return EncodeBase58(&vch[0], &vch[0] + vch.size());
}

// Decode a base58-encoded string psz into byte vector vchRet
// returns true if decoding is successful
inline bool DecodeBase58(const char* psz, std::vector<unsigned char>& vchRet)
{
    CAutoBN_CTX pctx;
    vchRet.clear();
    CBigNum bn58 = 58;
    CBigNum bn = 0;
    CBigNum bnChar;
    while (isspace(*psz))
        psz++;

    // Convert big endian string to bignum
    for (const char* p = psz; *p; p++)
    {
        const char* p1 = strchr(pszBase58, *p);
        if (p1 == NULL)
        {
            while (isspace(*p))
                p++;
            if (*p != '\0')
                return false;
            break;
        }
        bnChar.setulong(p1 - pszBase58);
        if (!BN_mul(&bn, &bn, &bn58, pctx))
            throw bignum_error("DecodeBase58 : BN_mul failed");
        bn += bnChar;
    }

    // Get bignum as little endian data
    std::vector<unsigned char> vchTmp = bn.getvch();

    // Trim off sign byte if present
    if (vchTmp.size() >= 2 && vchTmp.end()[-1] == 0 && vchTmp.end()[-2] >= 0x80)
        vchTmp.erase(vchTmp.end()-1);

    // Restore leading zeros
    int nLeadingZeros = 0;
    for (const char* p = psz; *p == pszBase58[0]; p++)
        nLeadingZeros++;
    vchRet.assign(nLeadingZeros + vchTmp.size(), 0);

    // Convert little endian data to big endian
    reverse_copy(vchTmp.begin(), vchTmp.end(), vchRet.end() - vchTmp.size());
    return true;
}

// Decode a base58-encoded string str into byte vector vchRet
// returns true if decoding is successful
inline bool DecodeBase58(const std::string& str, std::vector<unsigned char>& vchRet)
{
    return DecodeBase58(str.c_str(), vchRet);
}




// Encode a byte vector to a base58-encoded string, including checksum
inline std::string EncodeBase58Check(const std::vector<unsigned char>& vchIn)
{
    // add 4-byte hash check to the end
    std::vector<unsigned char> vch(vchIn);
    uint256 hash = Hash(vch.begin(), vch.end());
    vch.insert(vch.end(), (unsigned char*)&hash, (unsigned char*)&hash + 4);
    return EncodeBase58(vch);
}

// Decode a base58-encoded string psz that includes a checksum, into byte vector vchRet
// returns true if decoding is successful
inline bool DecodeBase58Check(const char* psz, std::vector<unsigned char>& vchRet)
{
    if (!DecodeBase58(psz, vchRet))
        return false;
    if (vchRet.size() < 4)
    {
        vchRet.clear();
        return false;
    }
    uint256 hash = Hash(vchRet.begin(), vchRet.end()-4);
    if (memcmp(&hash, &vchRet.end()[-4], 4) != 0)
    {
        vchRet.clear();
        return false;
    }
    vchRet.resize(vchRet.size()-4);
    return true;
}

// Decode a base58-encoded string str that includes a checksum, into byte vector vchRet
// returns true if decoding is successful
inline bool DecodeBase58Check(const std::string& str, std::vector<unsigned char>& vchRet)
{
    return DecodeBase58Check(str.c_str(), vchRet);
}





/** Base class for all base58-encoded data */
class CBase58Data
{
protected:
    // the version byte(s)
    std::vector<unsigned char> vchVersion;

    // the actually encoded data
    typedef std::vector<unsigned char, zero_after_free_allocator<unsigned char> > vector_uchar;
    vector_uchar vchData;

    CBase58Data()
    {
        vchVersion.clear();
        vchData.clear();
    }

    void SetData(const std::vector<unsigned char> &vchVersionIn, const void* pdata, size_t nSize)
    {
        vchVersion = vchVersionIn;
        vchData.resize(nSize);
        if (!vchData.empty())
            memcpy(&vchData[0], pdata, nSize);
    }

    void SetData(const std::vector<unsigned char> &vchVersionIn, const unsigned char *pbegin, const unsigned char *pend)
    {
        SetData(vchVersionIn, (void*)pbegin, pend - pbegin);
    }

public:
    bool SetString(const char* psz, unsigned int nVersionBytes = 1)
    {
        std::vector<unsigned char> vchTemp;
        DecodeBase58Check(psz, vchTemp);
        if (vchTemp.size() < nVersionBytes)
        {
            vchData.clear();
            vchVersion.clear();
            return false;
        }
        vchVersion.assign(vchTemp.begin(), vchTemp.begin() + nVersionBytes);
        vchData.resize(vchTemp.size() - nVersionBytes);
        if (!vchData.empty())
            memcpy(&vchData[0], &vchTemp[nVersionBytes], vchData.size());
        OPENSSL_cleanse(&vchTemp[0], vchData.size());
        return true;
    }

    bool SetString(const std::string& str)
    {
        return SetString(str.c_str());
    }

    std::string ToString() const
    {
        std::vector<unsigned char> vch = vchVersion;
        vch.insert(vch.end(), vchData.begin(), vchData.end());
        return EncodeBase58Check(vch);
    }

    int CompareTo(const CBase58Data& b58) const
    {
        if (vchVersion < b58.vchVersion) return -1;
        if (vchVersion > b58.vchVersion) return  1;
        if (vchData < b58.vchData)   return -1;
        if (vchData > b58.vchData)   return  1;
        return 0;
    }

    bool operator==(const CBase58Data& b58) const { return CompareTo(b58) == 0; }
    bool operator<=(const CBase58Data& b58) const { return CompareTo(b58) <= 0; }
    bool operator>=(const CBase58Data& b58) const { return CompareTo(b58) >= 0; }
    bool operator< (const CBase58Data& b58) const { return CompareTo(b58) <  0; }
    bool operator> (const CBase58Data& b58) const { return CompareTo(b58) >  0; }
};

/** base58-encoded addresses.
 * Public-key-hash-addresses have version 25 (or 111 testnet).
 * The data vector contains RIPEMD160(SHA256(pubkey)), where pubkey is the serialized public key.
 * Script-hash-addresses have version 85 (or 196 testnet).
 * The data vector contains RIPEMD160(SHA256(cscript)), where cscript is the serialized redemption script.
 */
class CBitcoinAddress;
class CBitcoinAddressVisitor : public boost::static_visitor<bool>
{
private:
    CBitcoinAddress *addr;
public:
    CBitcoinAddressVisitor(CBitcoinAddress *addrIn) : addr(addrIn) { }
    bool operator()(const CKeyID &id) const;
    bool operator()(const CScriptID &id) const;
    bool operator()(const CNoDestination &no) const;
};

class CBitcoinAddress : public CBase58Data
{
public:
    bool Set(const CKeyID &id) {
        SetData(Params().Base58Prefix(CChainParams::PUBKEY_ADDRESS), &id, 20);
        return true;
    }

    bool Set(const CScriptID &id) {
        SetData(Params().Base58Prefix(CChainParams::SCRIPT_ADDRESS), &id, 20);
        return true;
    }

    bool Set(const CTxDestination &dest)
    {
        return boost::apply_visitor(CBitcoinAddressVisitor(this), dest);
    }

    bool IsValid() const
    {
        bool fCorrectSize = vchData.size() == 20;
        bool fKnownVersion = vchVersion == Params().Base58Prefix(CChainParams::PUBKEY_ADDRESS) ||
                             vchVersion == Params().Base58Prefix(CChainParams::SCRIPT_ADDRESS);
        return fCorrectSize && fKnownVersion;
    }

    CBitcoinAddress()
    {
    }

    CBitcoinAddress(const CTxDestination &dest)
    {
        Set(dest);
    }

    CBitcoinAddress(const std::string& strAddress)
    {
        SetString(strAddress);
    }

    CBitcoinAddress(const char* pszAddress)
    {
        SetString(pszAddress);
    }

    CTxDestination Get() const {
        if (!IsValid())
            return CNoDestination();
        uint160 id;
        memcpy(&id, &vchData[0], 20);
        if (vchVersion == Params().Base58Prefix(CChainParams::PUBKEY_ADDRESS))
            return CKeyID(id);
        else if (vchVersion == Params().Base58Prefix(CChainParams::SCRIPT_ADDRESS))
            return CScriptID(id);
        else
            return CNoDestination();
    }

    bool GetKeyID(CKeyID &keyID) const {
        if (!IsValid() || vchVersion != Params().Base58Prefix(CChainParams::PUBKEY_ADDRESS))
            return false;
        uint160 id;
        memcpy(&id, &vchData[0], 20);
        keyID = CKeyID(id);
        return true;
    }

    bool IsScript() const {
        return IsValid() && vchVersion == Params().Base58Prefix(CChainParams::SCRIPT_ADDRESS);
    }
};

bool inline CBitcoinAddressVisitor::operator()(const CKeyID &id) const         { return addr->Set(id); }
bool inline CBitcoinAddressVisitor::operator()(const CScriptID &id) const      { return addr->Set(id); }
bool inline CBitcoinAddressVisitor::operator()(const CNoDestination &id) const { return false; }

/** A base58-encoded secret key */
class CBitcoinSecret : public CBase58Data
{
public:
    void SetKey(const CKey& vchSecret)
    {
        assert(vchSecret.IsValid());
        SetData(Params().Base58Prefix(CChainParams::SECRET_KEY), vchSecret.begin(), vchSecret.size());
        if (vchSecret.IsCompressed())
            vchData.push_back(1);
    }

    CKey GetKey()
    {
        CKey ret;
        ret.Set(&vchData[0], &vchData[32], vchData.size() > 32 && vchData[32] == 1);
        return ret;
    }

    bool IsValid() const
    {
        bool fExpectedFormat = vchData.size() == 32 || (vchData.size() == 33 && vchData[32] == 1);
        bool fCorrectVersion = vchVersion == Params().Base58Prefix(CChainParams::SECRET_KEY);
        return fExpectedFormat && fCorrectVersion;
    }

    bool SetString(const char* pszSecret)
    {
        return CBase58Data::SetString(pszSecret) && IsValid();
    }

    bool SetString(const std::string& strSecret)
    {
        return SetString(strSecret.c_str());
    }

    CBitcoinSecret(const CKey& vchSecret)
    {
        SetKey(vchSecret);
    }

    CBitcoinSecret()
    {
    }
};


template<typename K, int Size, CChainParams::Base58Type Type> class CBitcoinExtKeyBase : public CBase58Data
{
public:
    void SetKey(const K &key) {
        unsigned char vch[Size];
        key.Encode(vch);
        SetData(Params().Base58Prefix(Type), vch, vch+Size);
    }

    K GetKey() {
        K ret;
        ret.Decode(&vchData[0], &vchData[Size]);
        return ret;
    }

    CBitcoinExtKeyBase(const K &key) {
        SetKey(key);
    }

    CBitcoinExtKeyBase() {}
};

typedef CBitcoinExtKeyBase<CExtKey, 74, CChainParams::EXT_SECRET_KEY> CBitcoinExtKey;
typedef CBitcoinExtKeyBase<CExtPubKey, 74, CChainParams::EXT_PUBLIC_KEY> CBitcoinExtPubKey;

#endif // BITCOIN_BASE58_H
