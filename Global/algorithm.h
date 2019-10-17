#ifndef ALGORITHM_H
#define ALGORITHM_H
#include <QByteArray>
#include <QString>
#include <QTime>


class Algorithm
{
public:
    Algorithm();

    static unsigned short CalcCRC16(const QByteArray &datas);
    static void AppendCRC16(QByteArray &datas);
    static unsigned int CalcCRC32(const QByteArray &datas);

    static QByteArray UInt2ByteArray(uint num);

    /**
     * @brief 将uint型数字转换为字节数组，并填充到byteArray中
     *        按低字节在前转换
     * @param num 待转换uint型数字
     * @param byteArray 字节数组
     * @param startIndex 填充的起始位置(startIndex≥0 且startIndex小于数组大小)
     * @param nBytes 字节数(注意：nBytes>0 且nBytes≤4)
     */
    static void UInt2ByteArray(uint num, QByteArray& byteArray, int startIndex, int nBytes = 4);

    /**
     * @brief 将字节数组中指定位置开始的nBytes字节转换为uint型数字
     *        按低字节在前转换
     * @param byteArray 待转换的字节数组
     * @param startIndex 起始位置(startIndex≥0 且startIndex小于数组大小)
     * @param nBytes 字节数(注意：nBytes>0 且nBytes≤4)
     * @return 转换后的uint型数字
     */
    static uint ByteArray2UInt(const QByteArray& byteArray, int startIndex, int nBytes = 4);

    /**
     * @brief 将uint型数字转换为字节数组，并填充到byteArray中
     *        按高字节在前转换
     * @param num 待转换uint型数字
     * @param byteArray 字节数组
     * @param startIndex 填充的起始位置(startIndex≥0 且startIndex小于数组大小)
     * @param nBytes 字节数(注意：nBytes>0 且nBytes≤4)
     */
    static void UInt2ByteArrayR(uint num, QByteArray& byteArray, int startIndex, int nBytes = 4);

    /**
     * @brief 将字节数组中指定位置开始的nBytes字节转换为uint型数字
     *        按高字节在前转换
     * @param byteArray 待转换的字节数组
     * @param startIndex 起始位置(startIndex≥0 且startIndex小于数组大小)
     * @param nBytes 字节数(注意：nBytes>0 且nBytes≤4)
     * @return 转换后的uint型数字
     */
    static uint ByteArray2UIntR(const QByteArray& byteArray, int startIndex, int nBytes = 4);

    //将字符串转换为整数
    //支持10进制和16进制(16进制必须以0x开头)
    static int String2Int(const QString& strVal);
    //将整数转换为字符串
    //useHex为true时进行16进制转换，否则进行10进制转换
    static QString Int2String(int value, bool useHex=false);

    /**
     * @brief 将double型数据转化为字符串
     * @param realVal 待转化数据
     * @param nDecimals 小数位数：<0 自动；==0无小数位；>0指定小数位数
     * @param bRoundup ：true 四舍五入；false 不四舍五入
     * @return 转化后的字符串
     */
    static QString Double2String(double realVal, int nDecimals = -1, bool bRoundup = true);

    /**
     * @brief 将字节数组转换为（16进制）字符串
     * @param byteArray 待转换的字节数组
     * @param split true 使用空格分隔；false 不分隔
     * @return 转化后的字符串
     */
    static QString ByteArray2String(const QByteArray& byteArray, bool split=true);

    /**
     * @brief 将字节数组倒序转换为（16进制）字符串
     * @param byteArray 待转换的字节数组
     * @param split true 使用空格分隔；false 不分隔
     * @return 转化后的字符串
     */
    static QString ByteArray2StringR(const QByteArray& byteArray, bool split=true);

    /**
     * @brief 将字符串转换为字节数组
     * @param strInput 待转换的字符串
     * @param sep 分隔符
     * @param base 进制
     * @return 转换后的字节数组
     */
    static QByteArray String2ByteArray(const QString& strInput, const QString &sep = QString(" "), int base = 16);

    /**
     * @brief 根据CRC校验检查Modbus帧是否完整
     * @param frameDatas 待检查的帧数据
     * @return true 完整；false 不完整
     */
    static bool isModbusFrameComplete(const QByteArray& frameDatas);


    static QTime TimestampToTime(quint64 timestamp);
    static QString TimestampToString(quint64 timestamp);
};

#endif // ALGORITHM_H
