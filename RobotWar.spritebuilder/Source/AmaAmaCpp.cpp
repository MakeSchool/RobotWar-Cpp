#include "AmaAmaCpp.h"
#include <math.h>
#include <stdio.h>

AmaAmaCpp::AmaAmaCpp()
{
    this->currentState = AmaAmaCppAction::START;
    this->actionIndex = 0;
}

void AmaAmaCpp::run()
{
    this->actionIndex = 0;

    while (true)
    {
        while (this->currentState == AmaAmaCppAction::FIRING)
        {
            this->performNextFiringAction();
        }
        
        while (this->currentState == AmaAmaCppAction::SEARCHING)
        {
            this->performNextSearchingAction();
        }
        
        while (this->currentState == AmaAmaCppAction::DEFAULT)
        {
            this->performNextDefaultAction();
        }
        
        while (this->currentState == AmaAmaCppAction::START)
        {
            this->performNextStartAction();
        }
    }
}

void AmaAmaCpp::performNextStartAction()
{
    printf("ArenaSize: %f, %f\n", this->arenaDimensions().width, this->arenaDimensions().height);
    printf("robotBoundingBox: %f, %f\n", this->robotBoundingBox().size.width, this->robotBoundingBox().size.height);

    this->shoot();
    this->turnRobot(-90);
    this->moveAhead(500);
    printf("StartFin\n");
    this->setCurrentState(AmaAmaCppAction::SEARCHING);
}

void AmaAmaCpp::performNextDefaultAction()
{
    switch (this->actionIndex % 1)
    {
        case 0:
            this->moveAhead(100);
            break;
    }

    this->actionIndex++;
}

void AmaAmaCpp::performNextFiringAction()
{
    if (this->currentTimestamp() - this->lastKnownPositionTimestamp > 5.0f)
    {
        //Turn Gun to Head and Searching restart
        RWVec target(this->position().x + this->headingDirection().x, this->position().y + this->headingDirection().y);
        float angle = this->angleBetweenGunHeadingDirectionAndWorldPosition(target);
        printf("angle: %f\n", angle);
        this->turnGun(angle);
        this->setCurrentState(AmaAmaCppAction::SEARCHING);
    }
    else
    {
        printf("Fire!\n");
        this->shoot();
    }
}

void AmaAmaCpp::performNextSearchingAction()
{
    
    this->moveAhead(500);
    
//    switch (this->actionIndex % 4)
//    {
//        case 0: this->moveAhead(50); break;
//        case 1: this->turnRobotLeft(40); break;
//        case 2: this->moveAhead(50); break;
//        case 3: this->turnRobotRight(20); break;
//    }
    
    this->actionIndex++;
}

void AmaAmaCpp::gotHit()
{
    printf("GotHit!!\n");
    
    if (this->hitPoints() < 5)
    {
        this->cancelActiveAction();
        this->setCurrentState(AmaAmaCppAction::SEARCHING);
        this->moveAhead(200);
    }
}

void AmaAmaCpp::hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle)
{
    printf("HitWall! : %d\n", (int)side);
    if (this->currentState != AmaAmaCppAction::TURN_AROUND)
    {
        this->cancelActiveAction();
        
        this->setCurrentState(AmaAmaCppAction::TURN_AROUND);
        
        float angle;
        angle = fabs(hitAngle) / 2.0f;
        
        this->moveBack(50);
        this->turnRobot(90);
        this->setCurrentState(AmaAmaCppAction::SEARCHING);
    }
}

void AmaAmaCpp::bulletHitEnemy(RWVec enemyPosition)
{
    printf("HitEnemy!\n");
    if (this->currentState != AmaAmaCppAction::FIRING && this->currentState != AmaAmaCppAction::START)
    {
        this->cancelActiveAction();
        this->setCurrentState(AmaAmaCppAction::FIRING);
    }
    this->lastKnownPosition = enemyPosition;
    this->lastKnownPositionTimestamp = this->currentTimestamp();
}

void AmaAmaCpp::scannedRobotAtPosition(RWVec position)
{
    printf("Scan!\n");
    if (this->currentState != AmaAmaCppAction::FIRING)
    {
        this->cancelActiveAction();
        this->setCurrentState(AmaAmaCppAction::FIRING);
    }
    
    this->lastKnownPosition = position;
    this->lastKnownPositionTimestamp = this->currentTimestamp();
    
    this->Aim();
    this->shoot();
    this->shoot();
    this->shoot();
}

void AmaAmaCpp::setCurrentState(AmaAmaCppAction::AmaAmaCppAction newState)
{
    printf("newstagte: %d\n", newState);
    this->currentState = newState;
    this->actionIndex = 0;
    
    switch (newState) {
        case AmaAmaCppAction::SEARCHING :
        break;
            
        default:
            break;
    }
}

void AmaAmaCpp::turnGun(float angle)
{
    if (angle >= 0)
    {
        this->turnGunRight(fabsf(angle));
    }
    else
    {
        this->turnGunLeft(fabsf(angle));
    }
}

void AmaAmaCpp::turnRobot(float angle)
{
    if (angle >= 0)
    {
        this->turnRobotRight(fabsf(angle));
    }
    else
    {
        this->turnRobotLeft(fabsf(angle));
    }
}

void AmaAmaCpp::Aim()
{
    float angle = this->angleBetweenGunHeadingDirectionAndWorldPosition(this->lastKnownPosition);
    this->turnGun(angle);
}