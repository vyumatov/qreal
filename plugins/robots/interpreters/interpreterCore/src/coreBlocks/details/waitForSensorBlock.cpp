#include "waitForSensorBlock.h"

#include <interpreterBase/robotModel/robotParts/scalarSensor.h>

using namespace interpreterCore::coreBlocks::details;

WaitForSensorBlock::WaitForSensorBlock(interpreterBase::robotModel::RobotModelInterface &robotModel
//		, robots::enums::sensorType::SensorTypeEnum type
		)
	: WaitBlock(robotModel)
//	, mType(type)
{
}

WaitForSensorBlock::~WaitForSensorBlock()
{
}

void WaitForSensorBlock::run()
{
	QString port = stringProperty("Port");

	/// @todo Works only with scalar sensors.
	for (interpreterBase::robotModel::PortInfo portInfo : mRobotModel.availablePorts()) {
		if (portInfo.name() == port || portInfo.nameAliases().contains(port)) {
			interpreterBase::robotModel::robotParts::PluggableDevice *device
					= mRobotModel.configuration().pluggableDevice(portInfo
							, interpreterBase::robotModel::ConfigurationInterface::output);
			interpreterBase::robotModel::robotParts::ScalarSensor *sensor
					= static_cast<interpreterBase::robotModel::robotParts::ScalarSensor *>(device);
			if (sensor != nullptr) {
				mPort = portInfo;
				connect(sensor, &interpreterBase::robotModel::robotParts::ScalarSensor::newData
						, this, &WaitForSensorBlock::responseSlot);
				connect(sensor, &interpreterBase::robotModel::robotParts::AbstractSensor::failure
						, this, &WaitForSensorBlock::failureSlot);
				sensor->read();
				mActiveWaitingTimer.start();
			} else {
				mActiveWaitingTimer.stop();
				error(tr("%1 is not configured on port %2").arg(name(), portInfo.name()));
				return;
			}
		}
	}
}

//QList<Block::SensorPortPair> WaitForSensorBlock::usedSensors() const
//{
//	if (mType == robots::enums::sensorType::unused) {
//		return QList<SensorPortPair>();
//	}
//	robots::enums::inputPort::InputPortEnum const port = static_cast<robots::enums::inputPort::InputPortEnum>(intProperty("Port") - 1);
//	return QList<SensorPortPair>() << qMakePair(mType, static_cast<int>(port));
//}

void WaitForSensorBlock::timerTimeout()
{
	/// @todo True horror.
	interpreterBase::robotModel::robotParts::PluggableDevice *device
			= mRobotModel.configuration().pluggableDevice(mPort
					, interpreterBase::robotModel::ConfigurationInterface::output);

	interpreterBase::robotModel::robotParts::ScalarSensor *sensor
			= static_cast<interpreterBase::robotModel::robotParts::ScalarSensor *>(device);

	if (sensor) {
		sensor->read();
	}
}

void WaitForSensorBlock::stop()
{
	/// @todo True horror.
	interpreterBase::robotModel::robotParts::PluggableDevice *device
			= mRobotModel.configuration().pluggableDevice(mPort
					, interpreterBase::robotModel::ConfigurationInterface::output);

	interpreterBase::robotModel::robotParts::ScalarSensor *sensor
			= static_cast<interpreterBase::robotModel::robotParts::ScalarSensor *>(device);

	if (sensor) {
		disconnect(sensor, &interpreterBase::robotModel::robotParts::ScalarSensor::newData
				, this, &WaitForSensorBlock::responseSlot);

		disconnect(sensor, &interpreterBase::robotModel::robotParts::AbstractSensor::failure
				, this, &WaitForSensorBlock::failureSlot);
	}

	WaitBlock::stop();
}

void WaitForSensorBlock::stopActiveTimerInBlock()
{
	/// @todo True horror.
	interpreterBase::robotModel::robotParts::PluggableDevice *device
			= mRobotModel.configuration().pluggableDevice(mPort
					, interpreterBase::robotModel::ConfigurationInterface::output);

	interpreterBase::robotModel::robotParts::ScalarSensor *sensor
			= static_cast<interpreterBase::robotModel::robotParts::ScalarSensor *>(device);

	if (sensor) {
		disconnect(sensor, &interpreterBase::robotModel::robotParts::ScalarSensor::newData
				, this, &WaitForSensorBlock::responseSlot);

		disconnect(sensor, &interpreterBase::robotModel::robotParts::AbstractSensor::failure
				, this, &WaitForSensorBlock::failureSlot);
	}

	WaitBlock::stopActiveTimerInBlock();
}