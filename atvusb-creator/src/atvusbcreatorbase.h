//   -*- c++ -*------------------------------------------------------------
//
//   Author:            Scott Davilla / Stephan Diederich
//
//   Project name:      atvusb-creator
//
//   Date:              10/2008
//
//   Class:             AtvUsbCreatorBase
//
//   Base class:        
//
//   Derived class(es): AtvUsbCreator{Win32,OSX,Linux} 
//
//   Exceptions:        AtvUsbCreatorException (derived from std::runtime_error)
//
//   Description:       Base class for usb-creator functionality, specialized for the platform when needed
//   
//   Copyright (C) 2008  TeamXBMC
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the GNU General Public License
//   as published by the Free Software Foundation; either version 2
//   of the License, or (at your option) any later version.
//   
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//   
//   You should have received a copy of the GNU General Public License
//   along with this program; if not, write to the Free Software
//   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//   ---------------------------------------------------------------------- 

#ifndef ATVUSBCREATORBASE_H
#define ATVUSBCREATORBASE_H

#include <string>
#include <iostream>
#include <vector>
#include <stdexcept>

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QThread.h>
#include "lists.h"

// dummy helper to get access to QThreads protected sleep methods
struct qthread : public QThread
{
public:
 using QThread::sleep;
 using QThread::msleep;
 using QThread::usleep;
private:
  qthread();
};

struct AtvUsbCreatorException : public std::runtime_error{
  AtvUsbCreatorException(const std::string &fcr_message):std::runtime_error(fcr_message){
  }
};

//just a dummy for a proper logger
struct Logger {
  void error(const std::string& fcr_message){
    std::cerr << fcr_message << std::endl;
  }
};

//---------------------------------------------------------------------- 
class AtvUsbCreatorBase : public QObject
{
  Q_OBJECT;

public:
          AtvUsbCreatorBase();
  virtual ~AtvUsbCreatorBase();

  void setDrive(const QString &fcr_path);
  const QStringList& getcrDevices();
  void setDMGPath(const QString &fcr_path);
  
  const QString& getcrDMGPath() const;
  const QString& getcrBootEFIPath() const;
  const QString& getcrDownloadFolder();
  
  //get access to InfoData
  InfoData& getrInfoData();
  
  //get access to logger functionality
  Logger& logger();

  QString do_process(const QString &program, const QStringList &arguments);
  
  virtual void mount_disk();
  virtual void umount_disk();
  virtual void detect_removable_drives() = 0; //throws AtvUsbCreatorException
  virtual void extract_bootefi() = 0;
  virtual void extract_7z_image(QString const& fcr_archive_path, QString const& fcr_hfs_image_path, QString const& fcr_staging_folder) = 0;
  virtual void extract_recovery();
  virtual bool verify_image();
  virtual bool create_image();
  virtual bool partition_disk();
  virtual bool install_recovery();
  virtual bool install_patchstick();
  virtual bool install_payload(INSTALLER const& fcr_installer, QString const& fcr_mount_point);

signals:
  void progress(int);
  void maxprogress(int);
  void status(QString);
  
protected:
  struct tDeviceInfo{
    QString name;
    QString label;
    QString mount;
    bool mounted;
  };
  //gets populated in platform specific implementation
  QStringList             m_devices; //this is a QStringList for gui usage
  std::map<QString, tDeviceInfo> m_device_infos;
  QString                 m_drive;
  QString                 m_dmg_path;
  QString                 m_bootefi_path;
  QString                 m_tmp_folder;
  bool                    m_drive_efi;
  QString                 m_drive_recovery;
  QString                 m_drive_patchstick;
  
private:
  Logger                  m_logger;
  InfoData                m_info_data;
  // absolute paths to various items/folders

  QString                 m_payload_folder;
  QString                 m_staging_folder;
  QString                 m_download_folder;
};
#endif //ATVUSBCREATORBASE_H



