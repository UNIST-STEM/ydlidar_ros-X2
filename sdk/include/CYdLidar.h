﻿
#pragma once
#include "utils.h"
#include "ydlidar_driver.h"
#include <math.h>


#define PropertyBuilderByName(type, name, access_permission)\
    access_permission:\
        type m_##name;\
    public:\
    inline void set##name(type v) {\
        m_##name = v;\
    }\
    inline type get##name() {\
        return m_##name;\
}\


using namespace ydlidar;

class YDLIDAR_API CYdLidar {
  PropertyBuilderByName(float, MaxRange,
                        private) ///< 设置和获取激光最大测距范围
  PropertyBuilderByName(float, MinRange,
                        private) ///< 设置和获取激光最小测距范围
  PropertyBuilderByName(float, MaxAngle,
                        private) ///< 设置和获取激光最大角度, 最大值180度
  PropertyBuilderByName(float, MinAngle,
                        private) ///< 设置和获取激光最小角度, 最小值-180度
  PropertyBuilderByName(float, OffsetTime, private)

  PropertyBuilderByName(bool, FixedResolution,
                        private) ///< 设置和获取激光是否是固定角度分辨率
  PropertyBuilderByName(bool, Reversion,
                        private) ///< 设置和获取是否旋转激光180度
  PropertyBuilderByName(bool, AutoReconnect,
                        private) ///< 设置异常是否开启重新连接
  PropertyBuilderByName(bool, GlassNoise,
                        private) ///< whether to close glass noise
  PropertyBuilderByName(bool, SunNoise, private) ///< whether to close sun noise

  PropertyBuilderByName(int, SerialBaudrate,
                        private) ///< 设置和获取激光通讯波特率
  PropertyBuilderByName(std::string, SerialPort,
                        private) ///< 设置和获取激光端口号
  PropertyBuilderByName(std::vector<float>, IgnoreArray,
                        private) ///< 设置和获取激光剔除点
  PropertyBuilderByName(int, AbnormalCheckCount,
                        private) ///< Maximum number of abnormal checks


 public:
  CYdLidar(); //!< Constructor
  virtual ~CYdLidar();  //!< Destructor: turns the laser off.

  bool initialize();  //!< Attempts to connect and turns the laser on. Raises an exception on error.

  // Return true if laser data acquistion succeeds, If it's not
  bool doProcessSimple(LaserScan &scan_msg, bool &hardwareError);

  //Turn on the motor enable
  bool  turnOn();  //!< See base class docs
  //Turn off the motor enable and close the scan
  bool  turnOff(); //!< See base class docs

  //get fixed resolution node size
  int getFixedSize() const;

  /** Returns true if the device is in good health, If it's not*/
  bool getDeviceHealth() const;

  /** Returns true if the device information is correct, If it's not*/
  bool getDeviceInfo(int &type);

  /** Retruns true if the heartbeat function is set to heart is successful, If it's not*/
  bool checkHeartBeat() const;

  /** Retruns true if the scan frequency is set to user's frequency is successful, If it's not*/
  bool checkScanFrequency();

  //Turn off lidar connection
  void disconnecting(); //!< Closes the comms with the laser. Shouldn't have to be directly needed by the user

 protected:
  /** Returns true if communication has been established with the device. If it's not,
    *  try to create a comms channel.
    * \return false on error.
    */
  bool  checkCOMMs();

  /** Returns true if health status and device information has been obtained with the device. If it's not,
    * \return false on error.
    */
  bool  checkStatus();

  /** Returns true if the normal scan runs with the device. If it's not,
    * \return false on error.
    */
  bool checkHardware();

  /** returns true if the lidar data is normal, If it's not*/
  bool checkLidarAbnormal();



 private:
  bool isScanning;
  bool isConnected;
  YDlidarDriver *lidarPtr;

  uint32_t m_pointTime;
  uint64_t last_node_time;
  int m_FixedSize;

};	// End of class

