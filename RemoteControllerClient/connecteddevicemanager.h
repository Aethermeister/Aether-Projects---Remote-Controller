#ifndef CONNECTEDDEVICEMANAGER_H
#define CONNECTEDDEVICEMANAGER_H

#include <QtCore/QString>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>

struct ConnectedDevice
{
    ConnectedDevice(const QString& ip_address, const QString& port, const QString& display_name) :
        IPAddress(ip_address), Port(port), DisplayName(display_name)
    {}

    QJsonObject ToJson() const
    {
        QJsonObject device_object;
        device_object.insert("ip_address", IPAddress);
        device_object.insert("port", Port);
        device_object.insert("display_name", DisplayName);

        return device_object;
    }

    bool operator==(const ConnectedDevice& device) const
    {
        return
                (
                    IPAddress == device.IPAddress &&
                    Port == device.Port &&
                    DisplayName == device.DisplayName
                );
    }

    QString IPAddress;
    QString Port;
    QString DisplayName;
};

class ConnectedDeviceManager
{
public:
    ConnectedDeviceManager() = default;
    virtual ~ConnectedDeviceManager() = default;

protected:
    QVector<ConnectedDevice> ConnectedDevices();
    void SaveConnectedDevices(const QVector<ConnectedDevice>& connected_devices);

    void AddDevice(const ConnectedDevice& device);
    void RemoveDevice(const ConnectedDevice& device);

};

#endif // CONNECTEDDEVICEMANAGER_H
