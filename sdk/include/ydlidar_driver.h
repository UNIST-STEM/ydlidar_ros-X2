#ifndef YDLIDAR_DRIVER_H
#define YDLIDAR_DRIVER_H
#include <stdlib.h>
#include <map>
#include "serial.h"
#include "locker.h"
#include "thread.h"
#include "ydlidar_protocol.h"

#if !defined(__cplusplus)
#ifndef __cplusplus
#error "The YDLIDAR SDK requires a C++ compiler to be built"
#endif
#endif

using namespace std;
using namespace serial;

namespace ydlidar {

class YDlidarDriver {
 public:
  /**
  * A constructor.
  * A more elaborate description of the constructor.
  */
  YDlidarDriver();

  /**
  * A destructor.
  * A more elaborate description of the destructor.
  */
  virtual ~YDlidarDriver();

  /**
  * @brief 连接雷达 \n
    * 连接成功后，必须使用::disconnect函数关闭
    * @param[in] port_path    串口号
    * @param[in] fileMode    波特率，YDLIDAR雷达有以下几个波特率：
    *     115200 S2
    * @return 返回连接状态
  * @retval 0     成功
    * @retval < 0   失败
    * @note连接成功后，必须使用::disconnect函数关闭
    * @see 函数::YDlidarDriver::disconnect (“::”是指定有连接功能,可以看文档里的disconnect变成绿,点击它可以跳转到disconnect.)
    */
  result_t connect(const char *port_path, uint32_t baudrate);

  /**
  * @brief 断开雷达连接
    */
  void disconnect();

  /**
  * @brief 获取当前SDK版本号 \n
    * 静态函数
    * @return 返回当前SKD 版本号
    */
  static std::string getSDKVersion();

  /**
   * @brief 获取雷达列表
   * 静态函数
   * @return map:第一个参数是雷达别名, 第二个参数是当前串口号
   */
  static std::map<std::string, std::string> lidarPortList();

  /**
  * @brief 连接雷达状态 \n
    * @return 返回连接状态
  * @retval true     成功
    * @retval false    失败
    */
  bool isConnected() const;

  /**
   * @brief isScanning
   * @return
   */
  bool isScanning() const;

  /**
   * @brief getPointTime
   * @return
   */
  uint32_t getPointTime() const;

  /**
  * @brief 设置雷达异常自动重新连接 \n
  * @param[in] enable    是否开启自动重连:
  *     true	开启
  *	  false 关闭
  */
  void setAutoReconnect(const bool &enable);


  /**
  * @brief 开启扫描 \n
    * @param[in] force    扫描模式
    * @param[in] timeout  超时时间
    * @return 返回执行结果
    * @retval RESULT_OK       开启成功
    * @retval RESULT_FAILE    开启失败
  * @note 只用开启一次成功即可
    */
  result_t startScan(bool force = false, uint32_t timeout = DEFAULT_TIMEOUT) ;

  /**
  * @brief 关闭扫描 \n
    * @return 返回执行结果
    * @retval RESULT_OK       关闭成功
    * @retval RESULT_FAILE    关闭失败
    */
  result_t stop();


  /**
  * @brief 获取激光数据 \n
    * @param[in] nodebuffer 激光点信息
  * @param[in] count      一圈激光点数
    * @param[in] timeout    超时时间
    * @return 返回执行结果
    * @retval RESULT_OK       获取成功
    * @retval RESULT_FAILE    获取失败
  * @note 获取之前，必须使用::startScan函数开启扫描
    */
  result_t grabScanData(node_info *nodebuffer, size_t &count,
                        uint32_t timeout = DEFAULT_TIMEOUT) ;


  /**
  * @brief 补偿激光角度 \n
  * 把角度限制在0到360度之间
    * @param[in] nodebuffer 激光点信息
  * @param[in] count      一圈激光点数
    * @return 返回执行结果
    * @retval RESULT_OK       成功
    * @retval RESULT_FAILE    失败
  * @note 补偿之前，必须使用::grabScanData函数获取激光数据成功
    */
  result_t ascendScanData(node_info *nodebuffer, size_t count);

  /**
  * @brief 打开电机 \n
    * @return 返回执行结果
    * @retval RESULT_OK       成功
    * @retval RESULT_FAILE    失败
    */
  result_t startMotor();

  /**
  * @brief 关闭电机 \n
    * @return 返回执行结果
    * @retval RESULT_OK       成功
    * @retval RESULT_FAILE    失败
    */
  result_t stopMotor();

  /**
   * @brief flush
   */
  void flush();

 protected:

  /**
  * @brief 创建解析雷达数据线程 \n
  * @note 创建解析雷达数据线程之前，必须使用::startScan函数开启扫图成功
    */
  result_t createThread();


  /**
  * @brief 重新连接开启扫描 \n
  * @param[in] force    扫描模式
  * @param[in] timeout  超时时间
  * @return 返回执行结果
  * @retval RESULT_OK       开启成功
  * @retval RESULT_FAILE    开启失败
  * @note sdk 自动重新连接调用
  */
  result_t startAutoScan(bool force = false, uint32_t timeout = DEFAULT_TIMEOUT) ;


  /**
  * @brief 解包激光数据 \n
    * @param[in] node 解包后激光点信息
  * @param[in] timeout     超时时间
    */
  result_t waitPackage(node_info *node, uint32_t timeout = DEFAULT_TIMEOUT);

  /**
  * @brief 发送数据到雷达 \n
    * @param[in] nodebuffer 激光信息指针
    * @param[in] count      激光点数大小
  * @param[in] timeout      超时时间
  * @return 返回执行结果
    * @retval RESULT_OK       成功
  * @retval RESULT_TIMEOUT  等待超时
    * @retval RESULT_FAILE    失败
    */
  result_t waitScanData(node_info *nodebuffer, size_t &count,
                        uint32_t timeout = DEFAULT_TIMEOUT);

  /**
  * @brief 激光数据解析线程 \n
    */
  int cacheScanData();

  /**
  * @brief 发送数据到雷达 \n
    * @param[in] cmd 	 命名码
    * @param[in] payload      payload
  * @param[in] payloadsize      payloadsize
  * @return 返回执行结果
    * @retval RESULT_OK       成功
    * @retval RESULT_FAILE    失败
    */
  result_t sendCommand(uint8_t cmd, const void *payload = NULL,
                       size_t payloadsize = 0);

  /**
  * @brief 等待激光数据包头 \n
    * @param[in] header 	 包头
    * @param[in] timeout      超时时间
  * @return 返回执行结果
    * @retval RESULT_OK       获取成功
  * @retval RESULT_TIMEOUT  等待超时
    * @retval RESULT_FAILE    获取失败
  * @note 当timeout = -1 时, 将一直等待
    */
  result_t waitResponseHeader(lidar_ans_header *header,
                              uint32_t timeout = DEFAULT_TIMEOUT);

  /**
  * @brief 等待固定数量串口数据 \n
    * @param[in] data_count 	 等待数据大小
    * @param[in] timeout    	 等待时间
  * @param[in] returned_size   实际数据大小
  * @return 返回执行结果
    * @retval RESULT_OK       获取成功
  * @retval RESULT_TIMEOUT  等待超时
    * @retval RESULT_FAILE    获取失败
  * @note 当timeout = -1 时, 将一直等待
    */
  result_t waitForData(size_t data_count, uint32_t timeout = DEFAULT_TIMEOUT,
                       size_t *returned_size = NULL);

  /**
  * @brief 获取串口数据 \n
    * @param[in] data 	 数据指针
    * @param[in] size    数据大小
  * @return 返回执行结果
    * @retval RESULT_OK       获取成功
    * @retval RESULT_FAILE    获取失败
    */
  result_t getData(uint8_t *data, size_t size);

  /**
  * @brief 串口发送数据 \n
    * @param[in] data 	 发送数据指针
    * @param[in] size    数据大小
  * @return 返回执行结果
    * @retval RESULT_OK       发送成功
    * @retval RESULT_FAILE    发送失败
    */
  result_t sendData(const uint8_t *data, size_t size);

  /**
  * @brief 关闭数据获取通道 \n
    */
  void disableDataGrabbing();

  /**
  * @brief 设置串口DTR \n
    */
  void setDTR();

  /**
  * @brief 清除串口DTR \n
    */
  void clearDTR();


 public:
  bool     m_isConnected;  ///< 串口连接状体
  bool     m_isScanning;   ///< 扫图状态
  bool     isAutoReconnect;  ///< 异常自动从新连接
  bool     isAutoconnting;  ///< 是否正在自动连接中


  enum {
    DEFAULT_TIMEOUT = 2000,    /**< 默认超时时间. */
    DEFAULT_HEART_BEAT = 1000, /**< 默认检测掉电功能时间. */
    MAX_SCAN_NODES = 2048,	   /**< 最大扫描点数. */
    DEFAULT_TIMEOUT_COUNT = 3,
  };
  enum {
    YDLIDAR_F4 = 1, /**< F4雷达型号代号. */
    YDLIDAR_T1 = 2, /**< T1雷达型号代号. */
    YDLIDAR_F2 = 3, /**< F2雷达型号代号. */
    YDLIDAR_S4 = 4, /**< S4雷达型号代号. */
    YDLIDAR_G4 = 5, /**< G4雷达型号代号. */
    YDLIDAR_X4 = 6, /**< X4雷达型号代号. */
    YDLIDAR_F4PRO = 6, /**< F4PRO雷达型号代号. */
    YDLIDAR_G4C = 9, /**< G4C雷达型号代号. */

  };
  node_info      scan_node_buf[2048];  ///< 激光点信息
  size_t         scan_node_count;      ///< 激光点数
  Event          _dataEvent;			 ///< 数据同步事件
  Locker         _lock;				///< 线程锁
  Locker         _serial_lock;		///< 串口锁
  Thread 	       _thread;				///< 线程id

 private:
  serial::Serial *_serial;			///< 串口
  node_packages packages;

  float       IntervalSampleAngle;
  float       IntervalSampleAngle_LastPackage;
  int         PackageSampleBytes;             ///< 一个包包含的激光点数
  bool        isSupportMotorCtrl;			///< 是否支持电机控制
  bool        CheckSumResult;
  uint32_t    m_baudrate;					///< 波特率
  uint32_t    m_pointTime;			///< 两个激光点时间间隔
  uint32_t    trans_delay;				///< 串口传输一个byte时间
  uint16_t    package_Sample_Index;
  uint16_t    FirstSampleAngle;
  uint16_t    LastSampleAngle;
  uint16_t    CheckSum;
  uint16_t    CheckSumCal;
  uint16_t    SampleNumlAndCTCal;
  uint16_t    LastSampleAngleCal;
  uint16_t    Valu8Tou16;

  std::string serial_port;///< 雷达端口

};
}

#endif // YDLIDAR_DRIVER_H
