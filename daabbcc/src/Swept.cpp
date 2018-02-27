//
//  Swept.cpp
//  DAABBCC
//
//  Created by selimanac on 27.02.2018.
//  Copyright © 2018 Selim Anac. All rights reserved.
//
#include <iostream>
#include "Swept.hpp"



float Swept::SweptAABB(AABB box1, AABB box2, vector<float> vel1, float normalx,float normaly)
{
    // These variables stand for the distance in each axis between the moving object and the stationary object in terms of when the moving object would "enter" the colliding object.
    float xDistanceEntry, yDistanceEntry;
    
    // These variables stand for the distance in each axis in terms of when the moving object would "exit" the colliding object.
    float xDistanceExit, yDistanceExit;
    
    // Find the distance between the objects on the near and far sides for both x and y
    // Depending on the direction of the velocity, we'll reverse the calculation order to maintain the right sign (positive/negative).
    if (vel1[0] > 0.0f)
    {
        xDistanceEntry = (box2).lowerBound[0] - (box1).upperBound[0];
        xDistanceExit = (box2).upperBound[0] - (box1).lowerBound[0];
    }
    else
    {
        xDistanceEntry = (box2).upperBound[0] - (box1).lowerBound[0];
        xDistanceExit = (box2).lowerBound[0] - (box1).upperBound[0];
    }
    
    if (vel1[1] > 0.0f)
    {
        yDistanceEntry = (box2).lowerBound[1] - (box1).upperBound[1];
        yDistanceExit = (box2).upperBound[1] - (box1).lowerBound[1];
    }
    else
    {
        yDistanceEntry = (box2).upperBound[1] - (box1).lowerBound[1];
        yDistanceExit = (box2).lowerBound[1] - (box1).upperBound[1];
    }
    
    // These variables stand for the time at which the moving object would enter/exit the stationary object.
    float xEntryTime, yEntryTime;
    float xExitTime, yExitTime;
    
    // Find time of collision and time of leaving for each axis (if statement is to prevent divide by zero)
    if (vel1[0] == 0.0f)
    {
        // If the largest distance (entry or exit) between the two objects is greater than the size of both objects combined, then the objects are clearly not colliding.
        if (max(fabsf(xDistanceEntry), fabsf(xDistanceExit)) > (((box1).upperBound[0] - (box1).lowerBound[0]) + ((box2).upperBound[0] - (box2).lowerBound[0])))
        {
            // Setting this to 2.0f will cause an absence of collision later in this function.
            xEntryTime = 2.0f;
        }
        else
        {
            // Otherwise, pass negative infinity to basically ignore this variable.
            xEntryTime = -numeric_limits<float>::infinity();
        }
        
        // Setting this to postivie infinity will ignore this variable.
        xExitTime = numeric_limits<float>::infinity();
    }
    else
    {
        // If there is a velocity in the x-axis, then we can determine the time of collision based on the distance divided by the velocity. (Assuming velocity does not change.)
        xEntryTime = xDistanceEntry / vel1[0];
        xExitTime = xDistanceExit / vel1[0];
    }
    
    if (vel1[1] == 0.0f)
    {
        if (max(fabsf(yDistanceEntry), fabsf(yDistanceExit)) > (((box1).upperBound[1] - (box1).lowerBound[1]) + ((box2).upperBound[1] - (box2).lowerBound[1])))
        {
            yEntryTime = 2.0f;
        }
        else
        {
            yEntryTime = -std::numeric_limits<float>::infinity();
        }
        
        yExitTime = std::numeric_limits<float>::infinity();
    }
    else
    {
        yEntryTime = yDistanceEntry / vel1[1];
        yExitTime = yDistanceExit / vel1[1];
    }
    
    
    // Get the maximum entry time to determine the latest collision, which is actually when the objects are colliding. (Because all 3 axes must collide.)
    float entryTime = max(xEntryTime, yEntryTime);
    
    // Get the minimum exit time to determine when the objects are no longer colliding. (AKA the objects passed through one another.)
    float exitTime = min(xExitTime, yExitTime);
    
    // If anything in the following statement is true, there's no collision.
    // If entryTime > exitTime, that means that one of the axes is exiting the "collision" before the other axes are crossing, thus they don't cross the object in unison and there's no collison.
    // If all three of the entry times are less than zero, then the collision already happened (or we missed it, but either way..)
    // If any of the entry times are greater than 1.0f, then the collision isn't happening this update/physics step so we'll move on.
    if (entryTime > exitTime || (xEntryTime < 0.0f && yEntryTime < 0.0f)  || xEntryTime > 1.0f || yEntryTime > 1.0f)
    {
        // With no collision, we pass out zero'd normals.
        normalx = 0.0f;
        normaly = 0.0f;
        
        // If collision detection isn't working, try uncommenting the if statement and putting a break point on the std::cout statement.
        // Then you can check variable values within this algorithm to make sure everything is in order.
        /*if (glm::distance(obj1->GetPosition(), obj2->GetPosition()) < 0.1)
         {
         std::cout << "Something went wrong, and the objects are inside of each other but haven't been detected as a collision.";
         }*/
        
        // 2.0f signifies that there was no collision.
        return 2.0f;
    }
    else // If there was a collision
    {
        // Calculate normal of collided surface
        if (xEntryTime > yEntryTime) // If the x-axis is the last to cross, then that is the colliding axis.
        {
            if (xDistanceEntry < 0.0f) // Determine the normal based on positive or negative.
            {
                normalx = 1.0f;
                normaly = 0.0f;
            }
            else
            {
                normalx = -1.0f;
                normaly = 0.0f;
            }
        }
        else if (yEntryTime > xEntryTime)
        {
            if (yDistanceEntry < 0.0f)
            {
                normalx = 0.0f;
                normaly = 1.0f;
            }
            else
            {
                normalx = 0.0f;
                normaly = -1.0f;
            }
        }
         _normalx = normalx;
         _normaly = normaly;
        // Return the time of collision
        return entryTime;
    }
}
