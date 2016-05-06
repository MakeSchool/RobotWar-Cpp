#include "AmaAmaCpp.h"
#include <math.h>

AmaAmaCpp::AmaAmaCpp()
{
    this->currentState = AmaAmaCppAction::DEFAULT;
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
    }
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
        this->setCurrentState(AmaAmaCppAction::SEARCHING);
    }
    else
    {
        float angle = this->angleBetweenGunHeadingDirectionAndWorldPosition(this->lastKnownPosition);
        
        if (angle >= 0)
        {
            this->turnGunRight(fabsf(angle));
        }
        else
        {
            this->turnGunLeft(fabsf(angle));
        }
        
        this->shoot();
    }
}

void AmaAmaCpp::performNextSearchingAction()
{
    switch (this->actionIndex % 4)
    {
        case 0: this->moveAhead(50); break;
        case 1: this->turnRobotLeft(20); break;
        case 2: this->moveAhead(50); break;
        case 3: this->turnRobotRight(20); break;
    }
    
    this->actionIndex++;
}

void AmaAmaCpp::gotHit()
{
    this->shoot();
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
        
        if (hitAngle >= 0)
        {
            this->turnRobotLeft(fabs(hitAngle));
        }
        else
        {
            this->turnRobotRight(fabs(hitAngle));
        }
        
        this->moveAhead(20);
        
        this->setCurrentState(previousState);
    }
}

void AmaAmaCpp::bulletHitEnemy(RWVec enemyPosition)
{
    
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
}