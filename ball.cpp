#include "ball.h"

#include <QSGSimpleRectNode>
#include <QQuickWindow>
#include <qdebug.h>
#include <QTime>

Ball::Ball()
{
    mColor = getColor();
    mSpeed = 1;
    mRefreshingState = false;
    mRightPaddle = nullptr;
    mLeftPaddle = nullptr;

    setFlag(QQuickItem::ItemHasContents, true);

    connect(this, SIGNAL(windowChanged(QQuickWindow*)), this, SLOT(handleWindowChanged(QQuickWindow*)));

    resetBallMovement();

    qRegisterMetaType<Ball::player>("player");
}

Ball::~Ball()
{
    //Do nothing
}

void Ball::handleWindowChanged(QQuickWindow *win)
{
    //Signal void QQuickWindow::beforeSynchronizing()
    //This signal is emitted before the scene graph is synchronized with the QML state.
    //This signal can be used to do any preparation required before calls to QQuickItem::updatePaintNode().
    if (win) {
        connect(window(), &QQuickWindow::beforeSynchronizing, this, &Ball::updateGameState, Qt::DirectConnection);
    }
}

void Ball::updateGameState()
{
    detectPaddleCollision();
    detectWindowCollision();
    detectGoal();

    moveBall();

    update();
}

void Ball::setHorizontalMovement(horizontalMovement value)
{
    mHorizontalMovement = value;
}

Ball::horizontalMovement Ball::getHorizontalMovement()
{
    return mHorizontalMovement;
}

void Ball::setVerticalMovement(verticalMovement value)
{
     mVerticalMovement = value;
}

Ball::verticalMovement Ball::getVerticalMovement()
{
    return mVerticalMovement;
}

void Ball::setSpeed(qreal &speed)
{
    mSpeed = speed;
}

qreal Ball::getSpeed()
{
    return mSpeed;
}

void Ball::setColor(QColor& color)
{
    mColor = color;
}

QColor Ball::getColor()
{
    return mColor;
}

void Ball::setRefreshingState(bool& state)
{
    mRefreshingState = state;
}

bool Ball::getRefreshingState()
{
    return mRefreshingState;
}

void Ball::resetBallMovement()
{
    QTime time_seed = QTime::currentTime();

    //Vertical and horizontal movement is set randomly
    qsrand(static_cast<uint>(time_seed.msec()));

    bool leftMovement = (getRandInt(0, 100) % 2 == 0);
    bool upMovement = (getRandInt(0, 100) % 2 == 0);

    mHorizontalMovement = static_cast<horizontalMovement>(leftMovement);
    mVerticalMovement = static_cast<verticalMovement>(upMovement);
}

void Ball::detectPaddleCollision()
{
    //Nullptr prevention
    if (!mRightPaddle && !mLeftPaddle)
        return;

    //BALL
    //Ball top and bottom edge y coordinates
    int ballTopEdge    = qRound(y());
    int ballBottomEdge = qRound(y() + height());

    //Ball left and right edge x coordinates
    int ballLeftEdge   = qRound(x());
    int ballRightEdge  = qRound(x() + width());

    //Ball corners coordinates
    QPointF ballTopRightCorner    = QPointF(x() + width(), y());
    QPointF ballBottomRightCorner = QPointF(x() + width(), y() + height());

    //RIGHT PADDLE
    //Right paddle top and bottom edge y coordinates
    int rightPaddleTopEdge     = qRound(mRightPaddle->y());
    int rightPaddleBottomEdge  = qRound(mRightPaddle->y() + mRightPaddle->height());

    //Right paddle left and right edge x coordinates
    int rightPaddleLeftEdge    = qRound(mRightPaddle->x());
    int rightPaddleRightEdge   = qRound(mRightPaddle->x() + mRightPaddle->width());

    //Right paddle corners coordinates
    QPointF rightPaddleTopLeftCorner    = QPointF(mRightPaddle->x(), rightPaddleTopEdge);
    QPointF rightPaddleBottomLeftCorner = QPointF(mRightPaddle->x(), rightPaddleBottomEdge);

    //LEFT PADDLE
    //Left paddle top and bottom edge y coordinates
    int leftPaddleTopEdge    = qRound(mLeftPaddle->y());
    int leftPaddleBottomEdge = qRound(mLeftPaddle->y() + mLeftPaddle->height());

    //Left paddle left and right edge x coordinates
    int leftPaddleLeftEdge   = qRound(mLeftPaddle->x());
    int leftPaddleRightEdge  = qRound(mLeftPaddle->x() + mLeftPaddle->width());

    //Ball is moving right
    if (right == getHorizontalMovement()) {

        //Collision on corners at paddle's top
        if (ballBottomRightCorner == rightPaddleTopLeftCorner) {
            if (right == getHorizontalMovement())
                setHorizontalMovement(left);
            if (down == getVerticalMovement())
                setVerticalMovement(up);
        }

        //Collision on corners at paddle's bottom
        else if (ballTopRightCorner == rightPaddleBottomLeftCorner) {
            if (left == getHorizontalMovement())
                setHorizontalMovement(right);
            if (up == getVerticalMovement())
                setVerticalMovement(down);
        }

        //Collision on paddle's top edge
        else if (  ballBottomEdge    == rightPaddleTopEdge
                  &&(  ballLeftEdge   > rightPaddleLeftEdge
                     &&ballRightEdge <= rightPaddleRightEdge))
        {

            if (down == getVerticalMovement()) {
                setVerticalMovement(up);
            }
        }

        //Collision on paddle's bottom edge
        else if (  ballTopEdge      == rightPaddleBottomEdge
                 &&(  ballLeftEdge   > rightPaddleLeftEdge
                    &&ballRightEdge <= rightPaddleRightEdge))
        {
            if (up == getVerticalMovement()) {
                setVerticalMovement(down);
            }
        }

        //Collision on right paddle's left edge
        else if ((   ballTopEdge    >= rightPaddleTopEdge
                  && ballBottomEdge <= rightPaddleBottomEdge)

                  || (  ballTopEdge < rightPaddleTopEdge
                      && (  ballBottomEdge < rightPaddleBottomEdge
                          && ballBottomEdge > rightPaddleTopEdge))

                  || (  ballTopEdge > rightPaddleTopEdge
                      &&(  ballTopEdge < rightPaddleBottomEdge
                         && ballBottomEdge > rightPaddleBottomEdge)))
        {
            if (  ballRightEdge >= rightPaddleLeftEdge
                &&ballRightEdge < rightPaddleRightEdge) {
                setHorizontalMovement(left);
            }
        }
    }

    //Ball is moving left
    else if (left == getHorizontalMovement()) {

        if ((  ballTopEdge    >= leftPaddleTopEdge
             &&ballBottomEdge <= leftPaddleBottomEdge)

            || (  ballTopEdge       < leftPaddleTopEdge
                &&(  ballBottomEdge < leftPaddleBottomEdge
                   &&ballBottomEdge > leftPaddleTopEdge))

            || (ballTopEdge         > leftPaddleTopEdge
                &&(  ballTopEdge    < leftPaddleBottomEdge
                   &&ballBottomEdge > leftPaddleBottomEdge)))
        {
            if (  ballLeftEdge <= leftPaddleRightEdge
                &&ballLeftEdge > leftPaddleLeftEdge) {
                setHorizontalMovement(right);
            }
        }
    }
}

void Ball::detectWindowCollision()
{
    if (!window())
        return;

    //If ball hits floor or ceiling, vertical movement inverted
    if (y() <= 0) {
        if (up == getVerticalMovement())
            setVerticalMovement(down);
    }

    else if (y() >= window()->height() - height()) {
        if (down == getVerticalMovement())
            setVerticalMovement(up);
    }
}

void Ball::detectGoal()
{
    if (!window() || getRefreshingState())
        return;

    //Goal is detected if ball hits left or right wall
    if (x() <= 0)
        goalScoredChanged(human);

    else if (x() >= window()->width())
        goalScoredChanged(computer);
}

void Ball::moveBall()
{
    //Horizontal movement
    if (left == getHorizontalMovement())
        setX(x() - 1 * getSpeed());
    else
        setX(x() + 1 * getSpeed());

    //Vertical movement
    if (up == getVerticalMovement())
        setY(y() - 1 * getSpeed());
    else
        setY(y() + 1 * getSpeed());
}

int Ball::getRandInt(int low, int high)
{
    return qrand() % ((high + 1) - low) + low;
}

QSGNode *Ball::updatePaintNode(QSGNode *node, UpdatePaintNodeData *paint_data)
{
    Q_UNUSED(paint_data);

    //Handle ball's color
    QSGSimpleRectNode *rect = static_cast<QSGSimpleRectNode *>(node);
    if (!rect) {
        rect = new QSGSimpleRectNode();
        rect->setColor(getColor());
    }

    rect->setRect(boundingRect());

    return rect;
}

void Ball::setRightPaddle(QObject* aPaddle)
{
    mRightPaddle = static_cast<QQuickItem*>(aPaddle);
}

void Ball::setLeftPaddle(QObject* aPaddle)
{
    mLeftPaddle = static_cast<QQuickItem*>(aPaddle);
}
