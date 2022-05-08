#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_TransformedView.h"

namespace physics {
	struct Node : public olc::aabb::rect{
        olc::vd2d lastPos;
        bool locked = false;
        double mass = 1.0;
        bool updatedThisCycle = false;
        Node() {

        }
        Node(olc::vd2d p) {
            pos = p;
            lastPos = p;
            vel = olc::vd2d(0, 0);
            force = olc::vd2d(0, 0);
        }

        void Update(float& fElapsedTime) {
            olc::vd2d lastPosition = pos;
            if (!locked) {
                pos += (pos - lastPos) + (force) * (fElapsedTime * fElapsedTime / mass);
                vel = (pos - lastPos) / fElapsedTime;
            }
            lastPos = lastPosition;
        }

        void Lock() {
            locked = true;
            ResetForce();
            vel = olc::vd2d(0, 0);
            lastPos = pos;
        }
        void Unlock() { locked = false; }
        void ToggleLock() {
            locked ? Unlock() : Lock();
        }
        void ResetForce() { force = olc::vd2d(0, 0); }
    };

    struct Spring {
        double equilibriumDistance;
        Node* firstObject;
        Node* secondObject;
        Spring() {}
        Spring(Node* f, Node* s) {
            firstObject = f;
            secondObject = s;
            //equilibriumDistance = 30;
            equilibriumDistance = (secondObject->pos - firstObject->pos).mag();
        }
        void Update(float fElapsedTime, olc::vd2d gravitationalAcceleration, bool airResistance) {
            olc::vd2d delta = secondObject->pos - firstObject->pos;
            double deltaLength = delta.mag();
            delta *= (equilibriumDistance - deltaLength) / (equilibriumDistance * 1.0);
            if (!firstObject->locked)
                firstObject->pos -= delta * 0.2f * fElapsedTime;
            if (!secondObject->locked)
                secondObject->pos += delta * 0.2f * fElapsedTime;
        }
        void ApplyDamping(float dampingAmount, float fElapsedTime) {
            olc::vd2d springDifference = (firstObject->pos - secondObject->pos).norm();

            springDifference *= ((firstObject->pos - firstObject->lastPos - (secondObject->pos - secondObject->lastPos)).dot(springDifference)) * dampingAmount;
            if (!firstObject->locked)
                firstObject->lastPos += springDifference;
            if (!secondObject->locked)
                secondObject->lastPos -= springDifference;
        }

        void Draw(olc::TileTransformedView *tv) {
            tv->DrawLine(firstObject->pos, secondObject->pos);
        }
    };

    struct Rope {
        std::vector<Spring> springs;
        Rope() {
            //{ 2,5 }, {10,5}
            Node* first = new Node({ 2.0f, 5.0f });
            first->Lock();
            Node* second = new Node({ 2.05f, 5.0f });
            springs.push_back(Spring(first, second));
            for (float x = 2.1f; x < 10.0f; x+=0.5f) {
                first = second;
                second = new  Node({ x, 5.0f });
                springs.push_back(Spring(first, second));
            }
        }

        void Delete() {
            for (auto n: springs) {
                delete n.firstObject;
                delete n.secondObject;
            }
            springs.clear();
        }



        void Update(float fElapsedTime, olc::vd2d gravitationalAcceleration, bool airResistance) {

            for (auto spring : springs) {
                //first node
                if (!spring.firstObject->updatedThisCycle) {
                    if (!spring.firstObject->locked) {
                        //Gravity
                        spring.firstObject->ApplyForce(gravitationalAcceleration * spring.firstObject->mass);
                        //Air resistance
                        if (airResistance)
                            spring.firstObject->ApplyForce(spring.firstObject->vel * -0.1f * spring.firstObject->mass);
                    }
                    spring.firstObject->Update(fElapsedTime);
                    spring.firstObject->ResetForce();
                    spring.firstObject->updatedThisCycle = true;
                }
                if (!spring.secondObject->updatedThisCycle) {
                    //second node
                    if (!spring.secondObject->locked) {
                        //Gravity
                        spring.secondObject->ApplyForce(gravitationalAcceleration * spring.secondObject->mass);
                        //Air resistance
                        if (airResistance)
                            spring.secondObject->ApplyForce(spring.secondObject->vel * -0.1f * spring.secondObject->mass);
                    }
                    spring.secondObject->Update(fElapsedTime);
                    spring.secondObject->ResetForce();
                    spring.secondObject->updatedThisCycle = true;
                }
            }
            for (auto spring : springs) {
                spring.firstObject->updatedThisCycle = false;
                spring.secondObject->updatedThisCycle = false;
            }

            //Updating springs multiple times for stability
            for (uint16_t k = 0; k < 3; k++) {
                for (uint16_t i = 0; i < 8; i++) {
                    for (uint16_t j = 0; j < springs.size(); j++) {
                        springs.at(j).Update(fElapsedTime,gravitationalAcceleration,  airResistance);
                    }
                }

                for (uint16_t j = 0; j != springs.size(); j++) {
                    //float damping = pow(0.5f, fElapsedTime);
                    float damping = (1 - pow(0.0f, fElapsedTime)) * 0.5f;
                    springs.at(j).ApplyDamping(damping, fElapsedTime);
                }

            }
        }

        void Draw(olc::TileTransformedView *tv) {
            for (auto &spring: springs) {
                spring.Draw(tv);
            }
        }

    };

};