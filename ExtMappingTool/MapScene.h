#pragma once

#include <QtWidgets/QApplication>
class MapScene : public QGraphicsView
{
private:

public:
	MapScene(QGraphicsScene* pScene)
		: QGraphicsView(pScene, NULL)
	{}

	void AnimateBy(int x)
	{
		//test from
		float updateFrequency = (1000 / 30.0); // ~30 frames per second

		QPointF currScenePos = sceneRect().center();

		int curX = currScenePos.x();
		int endPos = curX + x;

		int distanceToAnimate = (endPos - curX);

		// speed = dist / time
		float updatePosInterval = (float)distanceToAnimate / updateFrequency;
		static float newXPos = sceneRect().center().x();

		QTimer* pTimer = new QTimer;
		QObject::connect(pTimer, &QTimer::timeout, [=]() {

			newXPos += updatePosInterval;
			centerOn(newXPos, sceneRect().center().y());

			// check for end position or time, then....
			if (newXPos >= endPos)
			{
				pTimer->stop();
				pTimer->deleteLater();
			}

		});
		pTimer->start(updateFrequency);
	}
};
