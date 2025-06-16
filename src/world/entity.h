#include "world.h"
#include "../input.h"


bool isWalkable(world* world, float x, float y) {
    // Convert global coordinates to chunk coordinates
    int global_x = (int)x;
    int global_y = (int)y;
    
    // Check world bounds
    int world_size_global = WORLD_SIZE * CHUNK_SIZE;
    if (global_x < 0 || global_x >= world_size_global || 
        global_y < 0 || global_y >= world_size_global) {
        return false; // Out of bounds
    }
    
    // Convert to chunk and cell coordinates
    int chunk_x = global_x / CHUNK_SIZE;
    int chunk_y = global_y / CHUNK_SIZE;
    int cell_x = global_x % CHUNK_SIZE;
    int cell_y = global_y % CHUNK_SIZE;
    
    // Check if the cell is walkable
    return world->chunks[chunk_x][chunk_y].cells[cell_x][cell_y].height == 0;
}
#define pi360 0.0165f
#define turnspeed 0.0165f*5
bool handleMovement(world* world) {
    bool moved = false;
    float SPEED= 0.1f;
    if (ispressed(run))
        SPEED = 0.2f;
    else
        SPEED = 0.1f;
    

    float dx = cosf(world->player.angle) * SPEED;
    float dy = sinf(world->player.angle) * SPEED;
    
    if (ispressed(forward)) {
        // Forward movement
        float new_x = world->player.x + dx;
        float new_y = world->player.y + dy;
        
        // Check X movement independently
        if (isWalkable(world, new_x, world->player.y)) {
            world->player.x = new_x;
        }
        
        // Check Y movement independently (allows sliding along walls)
        if (isWalkable(world, world->player.x, new_y)) {
            world->player.y = new_y;
        }
        moved = true;
    }
    
    if (ispressed(backward)) {
        // Backward movement
        float new_x = world->player.x - dx;
        float new_y = world->player.y - dy;
        
        // Check X movement independently
        if (isWalkable(world, new_x, world->player.y)) {
            world->player.x = new_x;
        }
        
        // Check Y movement independently
        if (isWalkable(world, world->player.x, new_y)) {
            world->player.y = new_y;
        }
        moved = true;
    }
    
    if (ispressed(strafeleft)) {
        // Strafe left (perpendicular to facing direction)
        float strafe_dx = -sinf(world->player.angle) * SPEED;
        float strafe_dy = cosf(world->player.angle) * SPEED;
        
        float new_x = world->player.x + strafe_dx;
        float new_y = world->player.y + strafe_dy;
        
        if (isWalkable(world, new_x, world->player.y)) {
            world->player.x = new_x;
        }
        
        if (isWalkable(world, world->player.x, new_y)) {
            world->player.y = new_y;
        }
        moved = true;
    }
    
    if (ispressed(straferight)) {
        // Strafe right
        float strafe_dx = sinf(world->player.angle) * SPEED;
        float strafe_dy = -cosf(world->player.angle) * SPEED;
        
        float new_x = world->player.x + strafe_dx;
        float new_y = world->player.y + strafe_dy;
        
        if (isWalkable(world, new_x, world->player.y)) {
            world->player.x = new_x;
        }
        
        if (isWalkable(world, world->player.x, new_y)) {
            world->player.y = new_y;
        }
        moved = true;
    }
    
    // Rotation
    if (ispressed(left)) {
        world->player.angle -= turnspeed;
        if (world->player.angle<0)world->player.angle+=6.28;
        moved = true;
    }
    
    if (ispressed(right)) {
        world->player.angle += turnspeed;
        if (world->player.angle>6.28)world->player.angle-=6.28;
        moved = true;
    }
    
    return moved;
}

// Alternative version with better collision detection (includes player radius)
void handleMovementWithRadius(world* world, float player_radius = 0.3f) {
    float SPEED= 1000.f;
    if (ispressed(run))
        SPEED = 2000.f;
    else
        SPEED = 1000.f;
    
    float dx = cosf(world->player.angle) * SPEED;
    float dy = sinf(world->player.angle) * SPEED;
    
    // Helper lambda to check if a circular area is walkable
    auto isAreaWalkable = [&](float center_x, float center_y) -> bool {
        // Check 4 corners of player's bounding box
        return isWalkable(world, center_x - player_radius, center_y - player_radius) &&
               isWalkable(world, center_x + player_radius, center_y - player_radius) &&
               isWalkable(world, center_x - player_radius, center_y + player_radius) &&
               isWalkable(world, center_x + player_radius, center_y + player_radius);
    };
    
    if (ispressed(forward)) {
        float new_x = world->player.x + dx;
        float new_y = world->player.y + dy;
        
        // Check X movement
        if (isAreaWalkable(new_x, world->player.y)) {
            world->player.x = new_x;
        }
        
        // Check Y movement
        if (isAreaWalkable(world->player.x, new_y)) {
            world->player.y = new_y;
        }
    }
    
    if (ispressed(backward)) {
        float new_x = world->player.x - dx;
        float new_y = world->player.y - dy;
        
        if (isAreaWalkable(new_x, world->player.y)) {
            world->player.x = new_x;
        }
        
        if (isAreaWalkable(world->player.x, new_y)) {
            world->player.y = new_y;
        }
    }
    
    // Add strafe and rotation as above...
}