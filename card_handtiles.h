#pragma once

#include <vector>
#include <sstream>

#include "card.h"

class HandTiles
{
    World& world;
    std::vector<std::pair<CardBase*, NPC*>> chosen; // 已选中的手牌
    std::vector<CardBase*> tiles;
    int max_size; // 手牌上限，用于决定是否抽牌，实际上有时tiles.size()会超过这个值
    CardBase* request_card(); // 抽一张新牌
    void clear()
    {
        for(auto [tile, _]: chosen) {
            delete tile;
        }
        for(auto tile: tiles) {
            delete tile;
        }
        chosen.clear(); tiles.clear();
    }

    void check_merge(int index); // 检查并合成(index, index+1)处可合成的牌，可能会递归

public:
    HandTiles(World &_world, int _max_size)
    : world(_world), max_size(_max_size)
    { }
    ~HandTiles() { clear(); }

    int choose(int index, NPC *main_t); // 选择手牌
    std::string summary(); // 手牌概览

    void set_max_size(int _max_size)
    {
        max_size = _max_size;
    }
    void reset(std::vector<CardBase*> _tiles)
    {
        clear();
        tiles = std::move(_tiles);
    }

    std::vector<CardBase*> dup_tiles() // 用于备份手牌状态
    {
        std::vector<CardBase*> dup;
        for(auto tile: tiles) {
            dup.push_back(tile->clone());
        }
        return dup;
    }

    auto get_chosen() // 获取选中的手牌
    {
        return chosen;
    }
    auto discharge() // 释放选中的手牌
    {
        std::vector<std::pair<CardBase*, NPC*>> ret = std::move(chosen);
        chosen.clear();
        draw();
        return ret;
    }
    void check_passion() // 处理激情与仪式
    {
        // TODO
    }

    void draw() // 抽牌
    {
        check_passion();
        while(tiles.size() < max_size) {
            tiles.insert(tiles.begin(), request_card());
            check_merge(0);
        }
    }
};