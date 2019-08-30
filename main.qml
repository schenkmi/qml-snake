import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import PongBall 1.0

ApplicationWindow {
    id: root;
    title: qsTr("Pong")
    width: Screen.width * 0.8
    height: 480
    visible: true
    color: "black";

    //Movement speed of paddles
    property int leftPaddleMovement: 40;
    property int rightPaddleMovement: 40;
    //Game speed
    property int gameSpeed: 6;
    //Score to achieve to win
    property int scoreToWin: 10;
    //Lap counter
    property int lap: 1;

    //Text to display lap, game speed and score to win
    Text {
        id: gameSpeedText;
        x: 10;
        y: 10;
        text: "Lap: " + lap + "\nGame speed: " + gameSpeed + "\nScore to win: " + scoreToWin;
        color: "white";
        font {
            pixelSize: 12;
        }
    }

    //Text to left player's (computer) score
    Text {
        id: leftScore;
        property int score: 0;
        anchors {
            top: parent.top;
            topMargin: 48;
        }
        x: root.width / 4
        text: leftScore.score;
        color: "white";
        font {
            pixelSize: 48;
        }
    }

    //Text to right player's (human) score
    Text {
        id: rightScore;
        property int score: 0;
        anchors {
            top: parent.top;
            topMargin: 48;
        }
        x: root.width / 2 + (root.width / 4)
        text: rightScore.score;
        color: "white";
        font {
            pixelSize: 48;
        }
    }

    //Center line
    Column {
        z: 2;
        anchors.centerIn: parent;
        spacing: 10;
        Repeater {
            id: repeater;
            model: root.height / (15 + parent.spacing) ;

            delegate: Rectangle {
                width: 5;
                height: 15;
                color: "white";
            }
        }
    }

    //Left paddle
    Rectangle {
        id: leftPaddle;
        z: 1;
        color: "white";
        anchors {
            left: parent.left;
            leftMargin: 100;
        }
        height: 80;
        y: root.height / 2;
        width: 24;

        Behavior on y {
            SmoothedAnimation {duration: 100;}
        }
    }

    //Right paddle
    Rectangle {
        id: rightPaddle;
        z: 1;
        color: "white";
        anchors {
            right: parent.right;
            rightMargin: 100;
        }
        x: root.width - 100
        focus: true;
        height: 80;
        width: 24;
        y: root.height / 2;

        //Key "Down" pressed
        Keys.onDownPressed: {
            if (rightPaddle.y < root.height - rightPaddle.height)
                rightPaddle.y += root.rightPaddleMovement;
        }

        //Key "Up" pressed
        Keys.onUpPressed: {
            if (rightPaddle.y > 0)
                rightPaddle.y -= root.rightPaddleMovement;
        }

        Behavior on y {
            SmoothedAnimation {duration: 100;}
        }
    }

    Ball {
        id: ball;
        height: 16;
        width: 16;
        color: "white";
        x: root.width / 2;
        y: root.height / 2;
        z: 1;
        speed: root.gameSpeed;

        Component.onCompleted: {
            setLeftPaddle(leftPaddle);
            setRightPaddle(rightPaddle);
        }

        onYChanged: {
            //Left paddle (computer) "follows" ball with limited velocity
            if (ball.refreshingState !== true) {
                var yCenterLeftPaddle = leftPaddle.y + leftPaddle.height/2;
                var yCenterBall       = ball.y+ball.height/2;
                var diffPaddleBall    = Math.abs(yCenterLeftPaddle - yCenterBall);

                var step;

                //Movement step size is equal to difference between paddle and ball
                //position but limited by a fixed value
                if (diffPaddleBall < root.leftPaddleMovement) {
                    step = diffPaddleBall;
                }
                else
                {
                    step = root.leftPaddleMovement;
                }

                //Move paddle
                if (yCenterLeftPaddle > yCenterBall) {
                    leftPaddle.y -= step;
                }
                else {
                    leftPaddle.y += step;
                }
            }
        }

        //Goal scored
        onGoalScoredChanged: {
            refreshingState = true;

            //Increase lap
            lap += 1;

            //reset left paddle's position
            leftPaddle.y = root.height / 2 - leftPaddle.height/2;

            //Figure out which player scored a goal
            //In QML only the integer value of an C++ enum can be handled
            if (goalGetter === 0)
                leftScore.score += 1;
            else if (goalGetter === 1)
                rightScore.score += 1;

            //Restart game
            gameReset.start();
        }
    }

    //Pause between laps
    Timer {
        id: gameReset;
        running: false;
        interval: 1000;
        onTriggered: {
            ball.x = root.width / 2;
            ball.y = root.height / 2;
            ball.resetBallMovement();
            ball.refreshingState = false;
            ball.visible = true;
        }
    }

    function handleGameState()
    {
        //Check if a player won or next lap has to start
        if (leftScore.score >= scoreToWin) {
            //Some action if left player wins
        }
        else if (rightScore.score >= scoreToWin) {
            //Some action if right player wins
        }
        else {
            //Increase game speed after some laps
            if (lap % 4 == 0)
            {
                gameSpeed += 1;
            }

            //Restart game
            gameReset.start();
        }
    }
}
