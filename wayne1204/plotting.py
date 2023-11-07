"""
python .\plotting.py --path ./circuit1/circuit1.conf --layer 2
"""

import numpy as np
import matplotlib.pyplot as plt
from matplotlib.collections import PatchCollection
from matplotlib.patches import Rectangle
import argparse
import os
import sys

class CircuitParser():
    def __init__(self):
        self.layers = [[] for item in range(9)]         # 非 critical net 的 polygon
        self.layers_fill = [[] for item in range(9)]    # fill polygon
        self.layers_cnet = [[] for item in range(9)]    # 属于 critical net 的 polygon
        self.boundary = []
        self.cnet = set()
        self.design = ''
        self.fill = ''
        fig, self.ax = plt.subplots(1)
    
    def parseCnet(self, fileName):
        """
        读取传入的 .conf 文件
        """
        text = open(fileName, 'r')
        for i, line in enumerate(text):
            if i == 0:
                ## 对应 design
                directory, fname = os.path.split(fileName)
                self.design = os.path.join(directory, line.split()[1])
                print('design name: ', self.design)
            elif i == 1:
                ## 对应 output
                directory, fname = os.path.split(fileName)
                self.fill = os.path.join(directory, line.split()[1])
                print('fill name: ', self.fill)
            elif i == 4:
                ## 对应 critical_nets
                row = line.split()[1:]
                num = [int(i) for i in row]
                self.cnet = set(num)

    def parseDesign(self, design = True): 
        """
        解析 .cut / .fill 文件
        """
        if design:
            text = open(self.design, 'r')
        else:
            text = open(self.fill, 'r')
        for i, line in enumerate(text):
            if i == 0 and design:
                ## 读取芯片边界
                row = line.replace(';', ' ').split(' ')
                self.boundary = [int(row[i])//1000 for i in range(4)]
                print('bounary (divided by 1000): ', self.boundary)
            else:
                row = line.split()
                idx = int(row[6]) - 1 # 当前 polygon 所在的 layer
                a = [int(row[i]) for i in range(1, 5)] # 当前 polygon 的坐标
                if int(row[5]) in self.cnet:
                    self.layers_cnet[idx].append(a) # 判断是否加入 critical net 列表中对应的 “层列表” 中
                elif design:
                    self.layers[idx].append(a)
                else:
                    self.layers_fill[idx].append(a)


    def insert_polygon(self, points, color='black'):
        errorboxes = []
        rect = Rectangle((points[0], points[1]), points[2] - points[0],
                        points[3] - points[1])
        errorboxes.append(rect)
        pc = PatchCollection(errorboxes, facecolor=color, alpha=0.5,
                             edgecolor=color)
        self.ax.add_collection(pc)
        artists = self.ax.errorbar(0, 0, xerr=0, yerr=0, fmt='None', ecolor='k')
    
    def scaling(self, num):
        """
        尺缩指定层（由 num 指定）下的 polygon 坐标
        """
        print(len(self.layers[num]))
        for j in range(len(self.layers[num])):
            self.layers[num][j] = [i/1000.0 for i in self.layers[num][j]]
        for j in range(len(self.layers_fill[num])):
            self.layers_fill[num][j] = [i/1000.0 for i in self.layers_fill[num][j]]
        for j in range(len(self.layers_cnet[num])):
            self.layers_cnet[num][j] = [i/1000.0 for i in self.layers_cnet[num][j]]
        print('scaling layer #{}...'.format(num+1))
        

    def adjustPoly(self, poly, lb_x, lb_y, rt_x, rt_y):
        """
        判断当前 poly 是否落于当前 window 中，同时并予以适当的阶段
        """
        ret = poly[:]
        if poly[0] > rt_x or poly[2] < lb_x:
            return [], 0
        if poly[1] > rt_y or poly[3] < lb_y:
            return [], 0

        if poly[0] < lb_x:
            ret[0] = lb_x
        if poly[1] < lb_y:
            ret[1] = lb_y
        if poly[2] > rt_x:
            ret[2] = rt_x
        if poly[3] > rt_y:
            ret[3] = rt_y
        return ret, (ret[2] - ret[0]) * (ret[3] - ret[1])

    def plot(self, num):
        """
        From the ICCAD 2018 CAD Contest: w is the window size for density calculation, and the step length is set to w/2
        但是此处似乎  step length is set to w  —— 待进一步思考 ？
        """
        WINDOW = 10
        HEIGHT = (self.boundary[3] - self.boundary[1]) // WINDOW
        WIDTH  = (self.boundary[2] - self.boundary[0]) // WINDOW
        for i in range(HEIGHT):
            for j in range(WIDTH):
                fig, self.ax = plt.subplots(1)

                ## 计算当前 window 坐标覆盖范围
                lb_x = self.boundary[0] + j * WINDOW
                lb_y = self.boundary[1] + i * WINDOW
                rt_x = self.boundary[0] + (j+1) * WINDOW
                rt_y = self.boundary[1] + (i+1) * WINDOW
                
                area_sum = 0.0
                print('window #{} / {}'.format(i * WIDTH + j+1, WIDTH * HEIGHT), end='\r')

                for k in range(len(self.layers[num])):
                    insert, area = self.adjustPoly(self.layers[num][k], lb_x, lb_y, rt_x, rt_y)
                    area_sum += area
                    if insert != []:
                        self.insert_polygon(insert, 'black')

                for points in self.layers_fill[num]:
                    insert, area = self.adjustPoly(points, lb_x, lb_y, rt_x, rt_y)
                    area_sum += area
                    if insert != []:
                        self.insert_polygon(insert, 'blue')

                for k in range(len(self.layers_cnet[num])):
                    insert, area = self.adjustPoly(self.layers_cnet[num][k], lb_x, lb_y, rt_x, rt_y)
                    area_sum += area
                    if insert != []:
                        self.insert_polygon(insert, 'red')

                density = area_sum/100.0
                plt.axis([lb_x, rt_x, lb_y, rt_y])
                # 输出密度 
                # plt.title('0:{} 1:{} 2:{} 3:{:.4f}'.format(num, i, j, density))
                plt.title('Layer_{}_{}_{}  density={:.4f}'.format(num+1, i, j, density))
                # plt.savefig("visualize/layer{}/layer_{}_{}_{}_{}.png".format(num+1,num+1, i//2, j//2,(i%2)*2+(j%2)))
                plt.savefig("visualize/layer{}/layer_{}_{}_{}.png".format(num+1,num+1, i, j))
                plt.close()
        print()

def ArgumentParser():
    parser = argparse.ArgumentParser("visualize circuit")
    parser.add_argument('--path', help='config file path')
    parser.add_argument('--layer', default=-1, type = int)
    return parser.parse_args()


def main():
    args = ArgumentParser()
    """
    print(args)

    output:

    usage: visualize circuit [-h] [--path PATH] [--layer LAYER]
    visualize circuit: error: argument --layer: invalid int value: '2.0'
    """
    cp = CircuitParser()
    cp.parseCnet(args.path)
    cp.parseDesign(True)  # 解析 .cut 文件
    cp.parseDesign(False) # 解析 .fill 文件

    if not os.path.exists('visualize'):
        os.mkdir('visualize')

    i = args.layer
    path = os.path.join('visualize', 'layer{}'.format(i))
    if not os.path.exists(path):
        os.mkdir(path)
    cp.scaling(i-1) # 尺缩坐标
    cp.plot(i-1) # 可视化

if __name__ == "__main__":
    main()
