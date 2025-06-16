#pragma once
#include"gr.h"


const color brick11 = color{ 51, 0, 0, 255 };
const color brick12 = color{ 102, 0, 0, 255 };
const color brick13 = color{ 153, 0, 0, 255 };
const color brick15 = color{ 255, 0, 0, 255 };
const color brick99 = color{ 255, 102, 102, 255 };

#define a (&brick11)
#define b (&brick12)
#define c (&brick13)
#define d (&brick15)
#define e (&brick99)

const color* bricktx[64] = {
    a, a, c, a, c, a, c, c,
    c, e, e, e, a, e, e, e,
    c, e, d, d, a, e, d, d,
    c, e, d, c, c, e, d, c,
    a, b, a, a, c, c, a, c,
    c, e, e, e, c, e, e, e,
    a, e, d, d, a, e, d, d,
    a, e, d, c, c, e, d, c,
};
#undef a
#undef b
#undef c
#undef d
#undef e



// grass
 color grass22 =  color{ 0, 102, 0, 255 };
 color grass28 =  color{ 0, 153, 0, 255 };
 color grass34 =  color{ 0, 204, 0, 255 };
 color grass77 =  color{ 51, 255, 51, 255 };
 color grass113 =  color{ 51, 255, 102, 255 };
 color grass151 =  color{ 153, 255, 153, 255 };
 color* grasscol[] = {
    &grass22, &grass28, &grass34, &grass77, &grass113, &grass151
};
#define a (grasscol[0])
#define b (grasscol[1])
#define c (grasscol[2])
#define d (grasscol[3])
#define e (grasscol[4])
#define f (grasscol[5])

const color* grasstx[64] = {
    a, d, b, f, c, e, a, d,
    b, e, c, d, a, d, b, e,
    c, f, a, e, d, b, c, f,
    a, e, d, b, f, c, a, e,
    d, b, f, c, a, e, d, b,
    f, c, a, d, e, b, f, c,
    a, d, b, a, e, c, a, d,
    b, e, c, f, d, a, b, e,
};
char grasstimer=0;
#undef a
#undef b
#undef c
#undef d
#undef e
#undef f



// lava
 color lava21 = color{ 255, 51, 0, 255 };
 color lava27 = color{ 255, 102, 0, 255 };
 color lava33 = color{ 255, 153, 0, 255 };
 color lava117 = color{ 255, 255, 102, 255 };
color * lavacol[] = {
    &lava21, &lava27, &lava33, &lava117
};
#define a (lavacol[3])
#define b (lavacol[2])
#define c (lavacol[1])
#define d (lavacol[1])
#define e (lavacol[0])

const color* lavatx[64] = {
    b, b, b, a, b, c, e, e,
    c, e, c, b, a, b, c, d,
    b, e, e, c, b, a, b, b,
    b, c, d, b, b, b, a, a,
    a, b, b, b, d, e, b, a,
    b, a, b, c, e, c, b, b,
    c, b, b, e, c, b, b, d,
    e, c, b, b, b, c, c, e,
};
int lavatimer=0;
#undef a
#undef b
#undef c
#undef d
#undef e



void updatetextures(){
    if ((grasstimer++)==5)
    {
        grasstimer=0;
    color tmp;
    tmp.r=(*grasscol[5]).r;
    tmp.g=(*grasscol[5]).g;
    tmp.b=(*grasscol[5]).b;
    tmp.a=(*grasscol[5]).a;
    for (int i=5;i>0;i--){
        grasscol[i]->r=grasscol[i-1]->r;
        grasscol[i]->g=grasscol[i-1]->g;
        grasscol[i]->b=grasscol[i-1]->b;
        grasscol[i]->a=grasscol[i-1]->a;
    }
    grasscol[0]->r=tmp.r;
    grasscol[0]->g=tmp.g;
    grasscol[0]->b=tmp.b;
    grasscol[0]->a=tmp.a;}
     
    if ((lavatimer++)==20)
    {
        lavatimer=0;
    color tmp;
    tmp.r=(*lavacol[3]).r;
    tmp.g=(*lavacol[3]).g;
    tmp.b=(*lavacol[3]).b;
    tmp.a=(*lavacol[3]).a;
    for (int i=3;i>0;i--){
        lavacol[i]->r=lavacol[i-1]->r;
        lavacol[i]->g=lavacol[i-1]->g;
        lavacol[i]->b=lavacol[i-1]->b;
        lavacol[i]->a=lavacol[i-1]->a;
    }
    lavacol[0]->r=tmp.r;
    lavacol[0]->g=tmp.g;
    lavacol[0]->b=tmp.b;
    lavacol[0]->a=tmp.a;}
    
}