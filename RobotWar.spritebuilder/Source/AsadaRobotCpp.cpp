//
//  AsadaRobotCpp.cpp
//  RobotWar
//
//  Created by Shuhei Asada on 2016/05/06.
//  Copyright © 2016年 MakeGamesWithUs. All rights reserved.
//

#include "AsadaRobotCpp.hpp"
#include <math.h>
#include <stdio.h>

void AsadaRobotCpp::run()
{
    init();
    
    this->turnGunLeft(30);
    
    while (true)
    {
        this->cancelActiveAction();
        
        switch (this->m_State)
        {
            case STATE_BACK:
            {
                switch (this->m_StateFrame)
                {
                    //case 0 :
                    //{
                    //
                    //}
                    //break;
                    case 0 : this->moveBack(999999); break;
                }
                break;
            }
                
            case STATE_AHEAD:
                switch (this->m_StateFrame)
                {
                    case 0 :
                    {
                        float angle = this->angleBetweenGunHeadingDirectionAndWorldPosition(this->gunHeadingDirection()) - this->angleBetweenHeadingDirectionAndWorldPosition(this->headingDirection());
                        
                        printf( "%lf¥r¥n", angle );
                        
                        if (angle > 29)
                        {
                            int anglen = fabs(angle) - 30;
                            if( anglen > 0 ){
                                this->turnGunRight( anglen );
                            }
                        }
                        else
                        {
                            int anglen = 30;
                            if( anglen > 0 ){
                                this->turnGunLeft( anglen );
                            }
                        }
                        
                        break;
                    }
                    case 1 : this->moveAhead(999999); break;
                }
                break;
                
            case STATE_TURNRIGHT90:
                switch (this->m_StateFrame)
                {
                    case 0 :  this->moveAhead(15); break;
                    case 1 :  this->turnRobotRight(90); break;
                    case 2 :  if( this->m_Next == STATE_NONE ){this->m_Next = STATE_AHEAD;} break;
                }
                break;
                
            case STATE_TURNLEFT90:
                switch (this->m_StateFrame)
                {
                    case 0 :  this->moveBack(15); break;
                    case 1 :  this->turnRobotLeft(90); break;
                    case 2 :  if( this->m_Next == STATE_NONE ){this->m_Next= STATE_AHEAD;} break;
                }
                break;
                
            case STATE_TURNENEMY:
                switch (this->m_StateFrame)
                {
                    case 0 :
                        if( this->m_Next == STATE_NONE )
                        {
                            float angle = this->angleBetweenGunHeadingDirectionAndWorldPosition(this->m_LastEnemyPosition);
                        
                            if (angle >= 0)
                            {
                                this->turnGunRight(fabsf(angle));
                            }
                            else
                            {
                                this->turnGunLeft(fabsf(angle));
                            }
                        }
                        break;
                    case 1 : this->shoot(); break;
                    case 2 : if( this->m_Next == STATE_NONE ){this->m_Next= STATE_AHEAD;} break;
                }
                break;
                
            case STATE_NONE:
                break;
                
            default:
                break;
        }
        
        if( this->m_Next != STATE_NONE )
        {
            this->m_State = this->m_Next;
            this->m_Next = STATE_NONE;
            this->m_StateFrame = 0;
        }
        else
        {
            this->m_StateFrame++;
        }
        
    }
}

void AsadaRobotCpp::gotHit()
{
    
}

void AsadaRobotCpp::hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle)
{
    this->cancelActiveAction();
    
    switch (side)
    {
        case RobotWallHitSide::FRONT:
            this->m_Next = State::STATE_TURNLEFT90;
            break;
            
        case RobotWallHitSide::REAR:
            this->m_Next = State::STATE_TURNRIGHT90;
            break;
            
        case RobotWallHitSide::LEFT:
            break;
            
        case RobotWallHitSide::RIGHT:
            break;
            
        case RobotWallHitSide::NONE:
            break;
    }
}

void AsadaRobotCpp::bulletHitEnemy(RWVec enemyPosition)
{
    //if( this->m_Next == STATE_NONE && ( this->m_State == STATE_AHEAD ) ){
    //    this->cancelActiveAction();
    //    this->m_LastEnemyPosition = enemyPosition;
    //    this->m_Next = STATE_TURNENEMY;
    //}
}

void AsadaRobotCpp::scannedRobotAtPosition(RWVec position)
{
    if( this->m_Next == STATE_NONE && ( this->m_State == STATE_AHEAD ) ){
        this->cancelActiveAction();
        this->m_LastEnemyPosition = position;
        this->m_Next = STATE_TURNENEMY;
    }
}

void AsadaRobotCpp::init()
{
    this->m_State = State::STATE_BACK;
    this->m_Next = STATE_NONE;
    this->m_StateFrame = 0;
}
