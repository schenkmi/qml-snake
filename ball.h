#ifndef BALL
#define BALL

#include <QQuickItem>
#include <QColor>
#include <QPoint>
#include <QtGlobal>
#include <QQuickWindow>

class Ball : public QQuickItem {
    Q_OBJECT
    Q_PROPERTY(QColor color READ getColor WRITE setColor)
    Q_PROPERTY(qreal speed READ getSpeed WRITE setSpeed)
    Q_PROPERTY(bool refreshingState READ getRefreshingState WRITE setRefreshingState)

private:
    typedef enum {
        up,
        down
    } verticalMovement;

    typedef enum {
        left,
        right
    } horizontalMovement;

    typedef enum {
        computer,
        human
    } player;

public:
    Ball();
    ~Ball();

    void setHorizontalMovement(horizontalMovement value);
    horizontalMovement getHorizontalMovement();

    void setVerticalMovement(verticalMovement value);
    verticalMovement getVerticalMovement();

    void setSpeed(qreal& speed);
    qreal getSpeed();

    void setColor(QColor &color);
    QColor getColor();

    void setRefreshingState(bool &state);
    bool getRefreshingState();

    Q_INVOKABLE void resetBallMovement();

private:
    void detectPaddleCollision();
    void detectWindowCollision();
    void detectGoal();
    void moveBall();
    int getRandInt(int low, int high);
    QSGNode *updatePaintNode(QSGNode *node, UpdatePaintNodeData *paint_data);

    QColor mColor;
    horizontalMovement mHorizontalMovement;
    verticalMovement mVerticalMovement;
    qreal mSpeed;
    bool mRefreshingState;

    QQuickItem *mRightPaddle;
    QQuickItem *mLeftPaddle;

public slots:
    void setRightPaddle(QObject *aPaddle);
    void setLeftPaddle(QObject *aPaddle);

private slots:
    void handleWindowChanged(QQuickWindow *win);
    void updateGameState();

signals:
    void goalScoredChanged(player goalGetter);
};

#endif // BALL
