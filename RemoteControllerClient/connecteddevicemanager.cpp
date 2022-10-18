#include "connecteddevicemanager.h"

#include <QStandardPaths>
#include <QDir>
#include <QFile>

constexpr const char* DevicesFile = "devices.json";

QVector<ConnectedDevice> ConnectedDeviceManager::ConnectedDevices()
{
    QVector<ConnectedDevice> connected_devices;

    const auto appdata_folder_path = QStandardPaths::standardLocations(QStandardPaths::StandardLocation::AppDataLocation).at(0);
    const auto devices_file_path = appdata_folder_path + "\\" + DevicesFile;

    QFile devices_file(devices_file_path);
    if(devices_file.open(QIODevice::ReadOnly))
    {
        const auto raw_file_content = devices_file.readAll();
        const auto file_content_document = QJsonDocument::fromJson(raw_file_content);
        const auto devices_array = file_content_document.array();

        for(const auto& device_value_ref : devices_array)
        {
            const auto device_object = device_value_ref.toObject();
            connected_devices.append({device_object.value("ip_address").toString(), device_object.value("port").toString(), device_object.value("display_name").toString()});
        }
    }

    return connected_devices;
}

void ConnectedDeviceManager::SaveConnectedDevices(const QVector<ConnectedDevice> &connected_devices)
{
    QJsonArray devices_array;

    for(const auto& connected_device : connected_devices)
    {
        devices_array.append(connected_device.ToJson());
    }

    QJsonDocument devices_document(devices_array);

    const auto appdata_folder_path = QStandardPaths::standardLocations(QStandardPaths::StandardLocation::AppDataLocation).at(0);
    const auto devices_file_path = appdata_folder_path + "\\" + DevicesFile;

    QDir appdata_folder(appdata_folder_path);
    if(!appdata_folder.exists())
    {
        appdata_folder.mkpath(appdata_folder_path);
    }

    QFile devices_file(devices_file_path);
    if(devices_file.open(QIODevice::WriteOnly))
    {
        devices_file.write(devices_document.toJson());
    }
}

void ConnectedDeviceManager::AddDevice(const ConnectedDevice &device)
{
    auto connected_devices = ConnectedDevices();
    connected_devices.append(device);

    SaveConnectedDevices(connected_devices);
}

void ConnectedDeviceManager::RemoveDevice(const ConnectedDevice &device)
{
    auto connected_devices = ConnectedDevices();
    connected_devices.removeOne(device);

    SaveConnectedDevices(connected_devices);
}
