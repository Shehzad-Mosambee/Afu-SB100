/*#########################################################################
                 Amoi Tech Mercury Application Build System
                 All Rights Reserved 2017
                 Author:
#########################################################################*/

#ifndef __MERCURY_SEC_H__
#define __MERCURY_SEC_H__
#include "MercuryDef.h"

/*************************************************************************************
 * 函数名：MercuryReadSecureFlag
 * 函数功能：查询是否已开启了secure boot

 * 返回:  true: secure boot enable
 *************************************************************************************/
extern BOOL MercuryReadSecureFlag(void);

/*************************************************************************************
 * 函数名：MercuryReadPubKey
 * 函数功能：读取公钥，已开安全的模块，从efuse中获取，未开启安全启动模块，从bootloader 读取。
 * out: 32字节长度的公钥。
 * 返回:  0
 *************************************************************************************/
extern int MercuryReadPubKey(uint8 *pubkey);

/*************************************************************************************
 * 函数名：MC_CryptoVerifyWithPk
 * 函数功能：校验接口，算法:blake2+ed25519
 * in:  msg 	验签数据
 		msglen 	数据长度 
 		sig 	签名数据 
 		pk 		公钥
 
 * 返回: 非0表示验签失败
 *************************************************************************************/

extern int MercuryCryptoVerifyWithPk(uint8 *msg, unsigned long long msglen, uint8 *sig, uint8 *pk);

/*************************************************************************************
 * 函数名：MC_CryptoVerify
 * 函数功能：校验接口，算法:blake2+ed25519，使用bootloader（未开安全）或者efuse(已开安全)中的公钥做验签。
 * in:  msg 	验签数据
 		msglen 	数据长度 
 		sig 	签名数据 
 
 * 返回: 非0表示验签失败
 *************************************************************************************/
extern int MercuryCryptoVerify(uint8 *msg, unsigned long long msglen, uint8 *sig);


#endif //__MERCURY_SEC_H__
