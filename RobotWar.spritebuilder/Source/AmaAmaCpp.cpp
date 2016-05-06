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
    printf("\nArenaSize: %f, %f", this->arenaDimensions().width, this->arenaDimensions().height);
    printf("\nrobotBoundingBox: %f, %f", this->robotBoundingBox().size.width, this->robotBoundingBox().size.height);

    this->setCurrentState(AmaAmaCppAction::SEARCHING);
    this->shoot();
    this->turnRobot(90);
    this->moveAhead(500);
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
    if (this->currentTimestamp() - this->lastKnownPositionTimestamp > 1.0f)
    {
        RWVec target(this->position().x + this->headingDirection().x, this->position().y + this->headingDirection().y);
        float angle = this->angleBetweenGunHeadingDirectionAndWorldPosition(target);
        printf("\nangle: %f", angle);
        this->turnGun(angle);
        this->setCurrentState(AmaAmaCppAction::SEARCHING);
    }
    else
    {
        float angle = this->angleBetweenGunHeadingDirectionAndWorldPosition(this->lastKnownPosition);
        this->turnGun(angle);
        this->shoot();
    }
}

void AmaAmaCpp::performNextSearchingAction()
{
    switch (this->actionIndex % 4)
    {
        case 0: this->moveAhead(50); break;
        case 1: this->turnRobotLeft(40); break;
        case 2: this->moveAhead(50); break;
        case 3: this->turnRobotRight(20); break;
    }
    
    this->actionIndex++;
}

void AmaAmaCpp::gotHit()
{
    //this->shoot();
    this->turnRobotLeft(45);
    this->moveAhead(100);
}

void AmaAmaCpp::hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle)
{
    if (this->currentState != AmaAmaCppAction::TURN_AROUND)
    {
        this->cancelActiveAction();
        
        AmaAmaCppAction::AmaAmaCppAction previousState = this->currentState;
        this->setCurrentState(AmaAmaCppAction::TURN_AROUND);
        
        float angle;
        angle = fabs(hitAngle) / 2.0f;
        
        this->turnRobot(angle);
        
        this->moveAhead(20);
        
        this->setCurrentState(previousState);
    }
}

void AmaAmaCpp::bulletHitEnemy(RWVec enemyPosition)
{
    if (this->currentState != AmaAmaCppAction::FIRING)
    {
        this->cancelActiveAction();
    }
    this->setCurrentState(AmaAmaCppAction::FIRING);
}

void AmaAmaCpp::scannedRobotAtPosition(RWVec position)
{
    if (this->currentState != AmaAmaCppAction::FIRING)
    {
        this->cancelActiveAction();
    }
    
    this->lastKnownPosition = position;
    this->lastKnownPositionTimestamp = this->currentTimestamp();
    
    this->setCurrentState(AmaAmaCppAction::FIRING);
    
}

void AmaAmaCpp::setCurrentState(AmaAmaCppAction::AmaAmaCppAction newState)
{
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