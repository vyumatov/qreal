#pragma once

#include <QtCore/QScopedPointer>

#include <interpreterBase/kitPluginInterface.h>
#include <commonTwoDModel/engine/twoDModelControlInterface.h>

#include "nxtAdditionalPreferences.h"
#include "robotModel/real/realRobotModel.h"
#include "robotModel/twoD/twoDRobotModel.h"

namespace nxtKitInterpreter {

class NxtKitInterpreterPlugin : public QObject, public interpreterBase::KitPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(interpreterBase::KitPluginInterface)
	Q_PLUGIN_METADATA(IID "nxtKitInterpreter.NxtKitInterpreterPlugin")

public:
	NxtKitInterpreterPlugin();

	QString kitId() const override;

	QString friendlyKitName() const override;

	QList<interpreterBase::robotModel::RobotModelInterface *> robotModels() override;

	interpreterBase::robotModel::RobotModelInterface *defaultRobotModel() override;

	// Transfers ownership.
	interpreterBase::AdditionalPreferences *settingsWidget() override;

//	qReal::IdList specificBlocks() const override;

	qReal::IdList unsupportedBlocks() const override;

	QList<qReal::ActionInfo> customActions() override;

private:
	robotModel::real::RealRobotModel mRealRobotModel;
	robotModel::twoD::TwoDRobotModel mTwoDRobotModel;
	NxtAdditionalPreferences *mAdditionalPreferences;  // Transfers ownership
	QScopedPointer<twoDModel::TwoDModelControlInterface> mTwoDModel;
};

}