/*
 * map.cpp
 *
 *  Created on: 2022年12月1日
 *      Author: Little Fish
 */

#include "map.h"
#include "debug.h"
#include "statusControl.h"
#include "math.h"
#include "stdio.h"

/*地图列表
 *
 * 7--8--9
 * |  |  |
 * |  |  |
 * 4--5--6
 * |  |  |
 * |  |  |
 * 1--2--3
 */
MapNode MapList[NodeNumber];

//第一项的节点A为原点 其余项必须以存在的节点开始延申
MapNodeRelationship NodeList[NodeRelationshipNumber] = {

{ 1, 2, Direction_Right, 5 },

{ 2, 3, Direction_Right, 2 },

{ 1, 4, Direction_Top, 2 },

{ 2, 5, Direction_Top, 2 },

{ 3, 6, Direction_Top, 2 },

{ 4, 5, Direction_Right, 5 },

{ 5, 6, Direction_Left, 2 },

{ 4, 7, Direction_Top, 2 },

{ 5, 8, Direction_Top, 2 },

{ 6, 9, Direction_Top, 2 },

{ 8, 7, Direction_Right, 5 },

{ 8, 9, Direction_Right, 2 }

};

MapNode *OpenList[NodeNumber];
MapNode *CloseList[NodeNumber];

//当前朝向
Direction CurrentDirection;
//起点
uint8_t StartPoint;

//计算两坐标距离
uint16_t CalculateDistance(MapNodePosition *A, MapNodePosition *B)
{
	uint16_t x = abs(A->X - B->X);
	uint16_t y = abs(A->X - B->X);

	return sqrt(x * x + y * y);
}

//计算 F G H
void CalculateFGH(MapNode *node, Direction direction, MapNode *EndPoint)
{

	if (node->ParentNode == 0)
	{
		node->H = CalculateDistance(&node->Position, &EndPoint->Position);
		node->F = node->H;
		return;
	}

	node->H = CalculateDistance(&node->Position, &EndPoint->Position);
	node->G = MapList[node->ParentNode].G + MapList[node->ParentNode].ChildNodeDistances[direction];
	node->F = node->H + node->G;

	//累计父节点距离
}

//查找路径 返回终点
MapNode* WayFinding(uint8_t end)
{
	MapNode *StartingNode = &MapList[StartPoint];
	MapNode *EndNode = &MapList[end];

	for (int i = 0; i < NodeNumber; i++)
	{
		CloseList[i] = OpenList[i] = NULL;
	}

	// 把起点加入 open list
	OpenList[StartingNode->Index] = StartingNode;
	CalculateFGH(StartingNode, Direction_None, EndNode);

	for (int k = 1; k < NodeNumber; k++)
	{
		int f_min = 99999;
		MapNode *currentNode = NULL;

		// 遍历 open list ，查找 F 值最小的节点，把它作为当前要处理的节点
		for (int i = 1; i < NodeNumber; i++)
		{
			if (OpenList[i] == NULL)
			{
				continue;
			}
			if (OpenList[i]->F < f_min)
			{
				f_min = OpenList[i]->F;
				currentNode = OpenList[i];
			}
		}

		//把这个节点移到 close list 。
		OpenList[currentNode->Index] = NULL;
		CloseList[currentNode->Index] = currentNode;

		//对当前方格的 8 个相邻方格的每一个方格？
		for (int i = 0; i < DirectionNumber; i++)
		{
			MapNode *childNode = &MapList[currentNode->ChildNodes[i]];

			if (childNode == NULL)
			{
				continue;
			}

			//如果它是不可抵达的或者它在 close list 中，忽略它
			if (CloseList[childNode->Index] != NULL)
			{
				continue;
			}

			if (OpenList[childNode->Index] == NULL)
			{
				//如果它不在 open list 中，把它加入 open list ，并且把当前方格设置为它的父亲，记录该方格的 F ， G 和 H 值

				OpenList[childNode->Index] = childNode;
				childNode->ParentNode = currentNode->Index;
				childNode->Direction = (Direction) i;
				CalculateFGH(childNode, (Direction) i, EndNode);
			}
			else
			{
				//如果它已经在 open list 中，检查这条路径 ( 即经由当前方格到达它那里 ) 是否更好，用 G 值作参考

				uint8_t flag = 1;
				for (int m = 1; m < NodeNumber; m++)
				{
					if (OpenList[m] == NULL)
					{
						continue;
					}
					if (OpenList[m]->G < childNode->G)
					{
						flag = 0;
						break;
					}
				}

				if (flag == 1)
				{
					childNode->ParentNode = currentNode->Index;
					childNode->Direction = (Direction) i;
					CalculateFGH(childNode, (Direction) i, EndNode);
				}
			}
		}

		//停止，当你
		if (OpenList[EndNode->Index] != NULL)
		{
			// 把终点加入到了 open list 中，此时路径已经找到了，或者
			break;
		}

		//    查找终点失败，并且 open list 是空的，此时没有路径。
		// 前提条件 一定存在路径
	}

	return EndNode;

}

//获取当前朝向 向何处转弯 才能调整为目标朝向
Direction GetDirectionDeviation(Direction current, Direction object)
{
	uint8_t dev_c = current == Direction_Top ? 0 : DirectionNumber - current;
	uint8_t dev_o = object == Direction_Top ? 0 : DirectionNumber - object;

	return (dev_c + dev_o) % DirectionNumber;

}

//从路线生成指令
//layer：递归层数 初始值0
//node：终点
uint8_t RoadToCommand(MapNode *node, uint8_t layer)
{
	if (node->ParentNode == 0)
	{
		//父节点为空即为起点
		printfX("%d", node->Index);
		return 0;
	}
	else
	{
		uint8_t reLayer = RoadToCommand(&MapList[node->ParentNode], layer + 1);
		Direction dir = node->ParentNode;
		if (reLayer == 0)
		{
			//起点需要考虑初始朝向偏差
			dir = GetDirectionDeviation(CurrentDirection, node->ParentNode);
		}
		printfX(" -- ");
		switch (dir)
		{
			case Direction_Left:
				printfX("左");
				Commands[reLayer].Duration = Command_Turn_Duration;
				Commands[reLayer].RunCommand_Time = 0;
				Commands[reLayer].Type = Command_Left;
				break;
			case Direction_Right:
				printfX("右");
				Commands[reLayer].Duration = Command_Turn_Duration;
				Commands[reLayer].RunCommand_Time = 0;
				Commands[reLayer].Type = Command_Right;
				break;
			case Direction_Top:
				printfX("上");
				Commands[reLayer].Duration = Command_Turn_Duration;
				Commands[reLayer].RunCommand_Time = 0;
				Commands[reLayer].Type = Command_Forward;
				break;
			case Direction_Bottom:
				printfX("下");
				Commands[reLayer].Duration = Command_Rotation_Duration;
				Commands[reLayer].RunCommand_Time = 0;
				Commands[reLayer].Type = Command_Rotation;
				break;
			default:
				break;
		}
		printfX(" --> ");
		printfX("%d", node->Index);

		//判断是否结束
		if (layer == 0)
		{
			Command_Number = reLayer + 1;
			printfX("\n\r");
		}
		return reLayer + 1;
	}

}

//画出地图
void GenerateMap()
{
	const int length = 30;
	//0 空地 -1 路 1 2 3 ....节点
	int map[length][length];
	for (int i = 0; i < length; i++)
	{
		for (int j = 0; j < length; j++)
		{
			map[i][j] = 0;
		}
	}
	int x = length / 2;
	int y = length / 2;

	for (int i = 0; i < NodeRelationshipNumber; i++)
	{
		//查找节点是否存在
		if (MapList[NodeList[i].NodeA].Index == 0)
		{
			if (i == 0)
			{
				MapList[NodeList[i].NodeA].Index = NodeList[i].NodeA;
				MapList[NodeList[i].NodeA].Position.X = x;
				MapList[NodeList[i].NodeA].Position.Y = y;
				//画第一个节点
				map[x][y] = NodeList[i].NodeA;
			}
			else
			{
				//报错 节点必须存在
				printfX("初始化地图错误，首节点必须存在\n\r");
			}
		}
		else
		{
			//修改当前坐标
			x = MapList[NodeList[i].NodeA].Position.X;
			y = MapList[NodeList[i].NodeA].Position.Y;
		}

		//计算路径
		int x_s = x;
		int y_s = y;
		switch (NodeList[i].Direction)
		{
			case Direction_Left:
				x_s -= NodeList[i].Distance + 1;
				MapList[NodeList[i].NodeA].ChildNodes[Direction_Left] = NodeList[i].NodeB;  //将另一端节点加入方向索引
				MapList[NodeList[i].NodeA].ChildNodeDistances[Direction_Left] = NodeList[i].Distance;
				MapList[NodeList[i].NodeB].ChildNodes[Direction_Right] = NodeList[i].NodeA;  //反向索引
				MapList[NodeList[i].NodeB].ChildNodeDistances[Direction_Right] = NodeList[i].Distance;
				break;
			case Direction_Right:
				x_s += NodeList[i].Distance + 1;
				MapList[NodeList[i].NodeA].ChildNodes[Direction_Right] = NodeList[i].NodeB;
				MapList[NodeList[i].NodeA].ChildNodeDistances[Direction_Right] = NodeList[i].Distance;
				MapList[NodeList[i].NodeB].ChildNodes[Direction_Left] = NodeList[i].NodeA;  //反向索引
				MapList[NodeList[i].NodeB].ChildNodeDistances[Direction_Left] = NodeList[i].Distance;
				break;
			case Direction_Top:
				y_s -= NodeList[i].Distance + 1;
				MapList[NodeList[i].NodeA].ChildNodes[Direction_Top] = NodeList[i].NodeB;
				MapList[NodeList[i].NodeA].ChildNodeDistances[Direction_Top] = NodeList[i].Distance;
				MapList[NodeList[i].NodeB].ChildNodes[Direction_Bottom] = NodeList[i].NodeA;  //反向索引
				MapList[NodeList[i].NodeB].ChildNodeDistances[Direction_Bottom] = NodeList[i].Distance;
				break;
			case Direction_Bottom:
				y_s += NodeList[i].Distance + 1;
				MapList[NodeList[i].NodeA].ChildNodes[Direction_Bottom] = NodeList[i].NodeB;
				MapList[NodeList[i].NodeA].ChildNodeDistances[Direction_Bottom] = NodeList[i].Distance;
				MapList[NodeList[i].NodeB].ChildNodes[Direction_Top] = NodeList[i].NodeA;  //反向索引
				MapList[NodeList[i].NodeB].ChildNodeDistances[Direction_Top] = NodeList[i].Distance;
				break;
			default:
				break;

		}

		//比较起止大小
		int x_max = x_s > x ? x_s : x;
		int x_min = x_s < x ? x_s : x;
		int y_max = y_s > y ? y_s : y;
		int y_min = y_s < y ? y_s : y;
		//画路径
		if (x == x_s)
		{
			for (int y_k = y_min + 1; y_k < y_max; y_k++)
			{
				map[x_s][y_k] = -1;
			}
		}
		if (y == y_s)
		{
			for (int x_k = x_min + 1; x_k < x_max; x_k++)
			{
				map[x_k][y_s] = -1;
			}
		}

		//画节点B
		x = x_s;
		y = y_s;
		map[x][y] = NodeList[i].NodeB;

		//将节点添加到列表中
		if (MapList[NodeList[i].NodeB].Index == 0)
		{
			MapList[NodeList[i].NodeB].Index = NodeList[i].NodeB;
			MapList[NodeList[i].NodeB].Position.X = x;
			MapList[NodeList[i].NodeB].Position.Y = y;
		}
	}

	//将画好的地图显示在屏幕上
	for (int j = 0; j < length; j++)
	{
		for (int i = 0; i < length; i++)
		{
			if (map[i][j] == -1)
			{
				if ((i - 1 < 0 || map[i - 1][j] == 0) && (i + 1 > length - 1 || map[i + 1][j] == 0))
				{
					printfX("| ");
				}
				else
				{
					printfX("--");
				}

			}
			else if (map[i][j] > 0)
			{
				printfX("%d ", map[i][j]);
			}
			else
			{
				printfX("  ");
			}

		}
		printfX("\n\r");
	}
}

//初始化地图
void InitMap()
{
	printfX("初始化地图\n\r");
	GenerateMap();
}

//设置缺省值
void SetDefaultValue(Direction currentDirection, uint8_t stratPoint)
{
	CurrentDirection = currentDirection;
	StartPoint = stratPoint;
}

//开始寻路并向终点运动
void StartWayFindingAndRuning(uint8_t end)
{
	//查找终点
	MapNode *EndNode = WayFinding(end);
	uint8_t layer = RoadToCommand(EndNode, 0);

	if (layer == 0)
	{
		printfX("起点与终点重合\n\r");
	}

	//开始运动
	Status_Control_Start(Command_Mode_OneTime, RuningSpeed);
}
