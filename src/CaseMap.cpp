/*

    Zelda Picross

    Copyright (C) 2010  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#include <sstream>

#include "CaseMap.h"
#include "StringTokeneizer.h"
#include "Resources.h"

CaseMap::CaseMap() : forceUp(false), forceDown(false), forceLeft(false), forceRight(false) {}
CaseMap::~CaseMap() {}
int CaseMap::getId() {return id;}
int CaseMap::getX() {return x;}
int CaseMap::getY() {return y;}
bool CaseMap::isUp() {return up;}
bool CaseMap::isDown() {return down;}
bool CaseMap::isLeft() {return left;}
bool CaseMap::isRight() {return right;}
bool CaseMap::isForceUp() {return forceUp;}
bool CaseMap::isForceDown() {return forceDown;}
bool CaseMap::isForceLeft() {return forceLeft;}
bool CaseMap::isForceRight() {return forceRight;}
CaseMapState CaseMap::getStatus() {return state;}
void CaseMap::setStatus(CaseMapState status) {state = status;}
void CaseMap::setForceUp(bool b) {forceUp = b;}
void CaseMap::setForceDown(bool b) {forceDown = b;}
void CaseMap::setForceLeft(bool b) {forceLeft = b;}
void CaseMap::setForceRight(bool b) {forceRight = b;}
