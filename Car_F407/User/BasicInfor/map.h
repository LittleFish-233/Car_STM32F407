/*
 * map.h
 *
 *  Created on: 2022年12月1日
 *      Author: Little Fish
 */

#ifndef BASICINFOR_MAP_H_
#define BASICINFOR_MAP_H_

#include "main.h"

#define NodeNumber 9+1
#define NodeRelationshipNumber 12
#define DirectionNumber 4
#define RuningSpeed 15

//地图节点位置
typedef struct
{
	uint16_t X;
	uint16_t Y;
} MapNodePosition;

typedef enum
{
	Direction_Top, Direction_Right, Direction_Bottom, Direction_Left, Direction_None
} Direction;

//地图节点关系及距离
typedef struct
{
	uint8_t NodeA;
	uint8_t NodeB;

	//B节点在A节点的方位
	Direction Direction;
	//两节点的距离
	uint16_t Distance;
} MapNodeRelationship;

/*
 *     上 Y
 *      |
 *      |
 *左---------右 X
 *      |
 *      |
 *      下
 */
//地图节点
typedef struct
{
	uint8_t Index;

//左边节点索引
	uint8_t ChildNodes[DirectionNumber];
	uint16_t ChildNodeDistances[DirectionNumber];

//节点位置
	MapNodePosition Position;

//父节点索引
	uint8_t ParentNode;
//相对父节点方向
	Direction Direction;

	uint16_t F;
	uint16_t G;
	uint16_t H;
} MapNode;

void InitMap();
void SetDefaultValue(Direction currentDirection, uint8_t stratPoint);
void StartWayFindingAndRuning(uint8_t end);

#endif /* BASICINFOR_MAP_H_ */
