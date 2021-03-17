//
// Created by seub on 3/17/21.
//

#pragma once

enum class TileResponse {
    OOB = -1,
    ALREADY_CREATED = 0,
    CREATED = 1,
    NOT_EMPTY = 2,
};

enum class PixelType {
    Air = 0,
    Sand = 1,
    Concrete = 2,
    Water = 3,
};