#include "assets.h"

/*
      ██████   
   ███   ███   
███      ███   
████████████   
         ███   
*/
const Buffer sprite_4 =
    (Buffer){.width = 8, .height = 5, .data = (uint8_t[]){0xc, 0xa, 0x9, 0xf, 0x8}};
/*
████████████   
         ███   
      ███      
   ███         
   ███         
*/
const Buffer sprite_7 =
    (Buffer){.width = 8, .height = 5, .data = (uint8_t[]){0xf, 0x8, 0x4, 0x2, 0x2}};
/*
   ███         ███   
███   ███   ███   ███
███      ███      ███
███               ███
   ███         ███   
      ███   ███      
         ███         
*/
const Buffer sprite_hearths =
    (Buffer){.width = 8, .height = 7, .data = (uint8_t[]){0x22, 0x55, 0x49, 0x41, 0x22, 0x14, 0x8}};
/*
   ██████      
███      ███   
████████████   
███      ███   
███      ███   
*/
const Buffer sprite_A =
    (Buffer){.width = 8, .height = 5, .data = (uint8_t[]){0x6, 0x9, 0xf, 0x9, 0x9}};
/*
                     
      ███   ███   ███
   ███   ███   ███   
      ███   ███   ███
   ███   ███   ███   
      ███   ███   ███
   ███   ███   ███   
*/
const Buffer sprite_pattern_small =
    (Buffer){.width = 8, .height = 7, .data = (uint8_t[]){0x0, 0x54, 0x2a, 0x54, 0x2a, 0x54, 0x2a}};
/*
                  ███                        
   ███      ████████████                     
   ████████████████████████      ███         
   ████████████████████████   ███   ███      
   ███      ████████████         ███         
                                             
         ███            ███                  
      ███   ███      ████████████      ███   
         ███      ████████████████████████   
                  ████████████████████████   
                     ████████████      ███   
                                             
                  ███            ███         
   ███      ████████████      ███   ███      
   ████████████████████████      ███         
   ████████████████████████            ███   
   ███      ████████████            ███      
                                    ███      
         ███         ███            ███      
      ███            ███   ███      ███      
      ███         ███      ███   ███         
*/
const Buffer sprite_pattern_big = (Buffer){
    .width = 16,
    .height = 21,
    .data = (uint8_t[]){0x40, 0x0, 0xf2, 0x0,  0xfe, 0x9,  0xfe, 0x15, 0xf2, 0x8,  0x0,
                        0x0,  0x8, 0x1,  0x94, 0x27, 0xc8, 0x3f, 0xc0, 0x3f, 0x80, 0x27,
                        0x0,  0x0, 0x40, 0x8,  0xf2, 0x14, 0xfe, 0x9,  0xfe, 0x21, 0xf2,
                        0x10, 0x0, 0x10, 0x88, 0x10, 0x84, 0x12, 0x44, 0xa}};
/*
                                          █████████████████████████████████████████████            
                                       ███                                             ███         
                                       █████████████████████████████████████████████   ███         
                                    ███                                             ██████         
                                    █████████████████████████████████████████████   ██████         
                                 ███                                             █████████         
                                 ███   ███      ███               ███            █████████         
                                 ███   ███   ███   ███         █████████         █████████         
                                 █████████████████████████████████████████████   █████████         
                              ███                                             ████████████         
                              ███   ███      ███               ███            ████████████         
                              ███   ███   ███   ███         █████████         ████████████         
            ███████████████████████████████████████████████████████████████   ████████████         
         ███                                             ███               ███████████████         
         ███   ███      ███               ███            ██████            ███████████████         
         ███   ███   ███   ███         █████████         █████████         ███████████████         
         ███   ███   ███   ███      ███████████████      ███████████████   ███████████████         
         ███   ███   ███   ███   █████████████████████   ██████         ██████████████████         
         ███   ███   ███   ███      ███████████████      █████████      ██████████████████         
         ███   ███      ███            █████████         █████████      ██████████████████         
         ███                              ███            █████████      ██████████████████         
         ███                                             ████████████   ███████████████            
         ███                                             ████████████   ███████████████            
         ███                                             ███████████████████████████               
         ███                                             ███████████████████████████               
         ███            ███                              ████████████████████████                  
         ███         █████████            ███      ███   ████████████████████████                  
         ███      ███████████████      ███   ███   ███   ████████████████████████                  
         ███   █████████████████████   ███   ███   ███   ████████████████████████                  
         ███      ███████████████      ███   ███   ███   █████████████████████                     
         ███         █████████         ███   ███   ███   █████████████████████                     
         ███            ███               ███      ███   █████████████████████                     
         ███                                             █████████████████████                     
            █████████████████████████████████████████████   ███████████████                        
            ███                                             ███████████████                        
               █████████████████████████████████████████████   ████████████                        
               ███            ███               ███      ███   ████████████                        
               ███                                             █████████                           
                  █████████████████████████████████████████████   ██████                           
                  ███            ███               ███      ███   ██████                           
                  ███                                             ██████                           
                     █████████████████████████████████████████████   ███                           
                     ███                                             ███                           
                        █████████████████████████████████████████████                              
*/
const Buffer sprite_main_image = (Buffer){
    .width = 40,
    .height = 44,
    .data = (uint8_t[]){
        0x0,  0xc0, 0xff, 0x1f, 0x0, 0x0,  0x20, 0x0,  0x20, 0x0, 0x0,  0xe0, 0xff, 0x2f, 0x0,
        0x0,  0x10, 0x0,  0x30, 0x0, 0x0,  0xf0, 0xff, 0x37, 0x0, 0x0,  0x8,  0x0,  0x38, 0x0,
        0x0,  0x28, 0x41, 0x38, 0x0, 0x0,  0xa8, 0xe2, 0x38, 0x0, 0x0,  0xf8, 0xff, 0x3b, 0x0,
        0x0,  0x4,  0x0,  0x3c, 0x0, 0x0,  0x94, 0x20, 0x3c, 0x0, 0x0,  0x54, 0x71, 0x3c, 0x0,
        0xf0, 0xff, 0xff, 0x3d, 0x0, 0x8,  0x0,  0x8,  0x3e, 0x0, 0x28, 0x41, 0x18, 0x3e, 0x0,
        0xa8, 0xe2, 0x38, 0x3e, 0x0, 0xa8, 0xf2, 0xf9, 0x3e, 0x0, 0xa8, 0xfa, 0x1b, 0x3f, 0x0,
        0xa8, 0xf2, 0x39, 0x3f, 0x0, 0x28, 0xe1, 0x38, 0x3f, 0x0, 0x8,  0x40, 0x38, 0x3f, 0x0,
        0x8,  0x0,  0x78, 0x1f, 0x0, 0x8,  0x0,  0x78, 0x1f, 0x0, 0x8,  0x0,  0xf8, 0xf,  0x0,
        0x8,  0x0,  0xf8, 0xf,  0x0, 0x8,  0x1,  0xf8, 0x7,  0x0, 0x88, 0x43, 0xfa, 0x7,  0x0,
        0xc8, 0xa7, 0xfa, 0x7,  0x0, 0xe8, 0xaf, 0xfa, 0x7,  0x0, 0xc8, 0xa7, 0xfa, 0x3,  0x0,
        0x88, 0xa3, 0xfa, 0x3,  0x0, 0x8,  0x41, 0xfa, 0x3,  0x0, 0x8,  0x0,  0xf8, 0x3,  0x0,
        0xf0, 0xff, 0xf7, 0x1,  0x0, 0x10, 0x0,  0xf0, 0x1,  0x0, 0xe0, 0xff, 0xef, 0x1,  0x0,
        0x20, 0x4,  0xe9, 0x1,  0x0, 0x20, 0x0,  0xe0, 0x0,  0x0, 0xc0, 0xff, 0xdf, 0x0,  0x0,
        0x40, 0x8,  0xd2, 0x0,  0x0, 0x40, 0x0,  0xc0, 0x0,  0x0, 0x80, 0xff, 0xbf, 0x0,  0x0,
        0x80, 0x0,  0x80, 0x0,  0x0, 0x0,  0xff, 0x7f, 0x0,  0x0}};
/*
   ██████      
███      ███   
███      ███   
███   ███      
   ███   ███   
*/
const Buffer sprite_Q =
    (Buffer){.width = 8, .height = 5, .data = (uint8_t[]){0x6, 0x9, 0x9, 0x5, 0xa}};
/*
   ██████      
███      ███   
      ███      
   ███         
████████████   
*/
const Buffer sprite_2 =
    (Buffer){.width = 8, .height = 5, .data = (uint8_t[]){0x6, 0x9, 0x4, 0x2, 0xf}};
/*
█████████      
         ███   
   ██████      
         ███   
█████████      
*/
const Buffer sprite_3 =
    (Buffer){.width = 8, .height = 5, .data = (uint8_t[]){0x7, 0x8, 0x6, 0x8, 0x7}};
/*
███      ███   
███   ███   ███
███   ███   ███
███   ███   ███
███      ███   
*/
const Buffer sprite_10 =
    (Buffer){.width = 8, .height = 5, .data = (uint8_t[]){0x9, 0x15, 0x15, 0x15, 0x9}};
/*
███      ███   
███   ███      
██████         
███   ███      
███      ███   
*/
const Buffer sprite_K =
    (Buffer){.width = 8, .height = 5, .data = (uint8_t[]){0x9, 0x5, 0x3, 0x5, 0x9}};
/*
████████████   
███            
█████████      
         ███   
████████████   
*/
const Buffer sprite_5 =
    (Buffer){.width = 8, .height = 5, .data = (uint8_t[]){0xf, 0x1, 0x7, 0x8, 0xf}};
/*
   ██████      
███            
█████████      
███      ███   
   ██████      
*/
const Buffer sprite_6 =
    (Buffer){.width = 8, .height = 5, .data = (uint8_t[]){0x6, 0x1, 0x7, 0x9, 0x6}};
/*
   ██████      
███      ███   
   █████████   
         ███   
   ██████      
*/
const Buffer sprite_9 =
    (Buffer){.width = 8, .height = 5, .data = (uint8_t[]){0x6, 0x9, 0xe, 0x8, 0x6}};
/*
         ███         
      ███   ███      
   ███         ███   
███               ███
   ███         ███   
      ███   ███      
         ███         
*/
const Buffer sprite_diamonds =
    (Buffer){.width = 8, .height = 7, .data = (uint8_t[]){0x8, 0x14, 0x22, 0x41, 0x22, 0x14, 0x8}};
/*
████████████   
███      ███   
████████████   
███      ███   
████████████   
*/
const Buffer sprite_8 =
    (Buffer){.width = 8, .height = 5, .data = (uint8_t[]){0xf, 0x9, 0xf, 0x9, 0xf}};
/*
         ███   
         ███   
         ███   
███      ███   
   ██████      
*/
const Buffer sprite_J =
    (Buffer){.width = 8, .height = 5, .data = (uint8_t[]){0x8, 0x8, 0x8, 0x9, 0x6}};
/*
         ███         
      █████████      
   ███████████████   
█████████████████████
██████   ███   ██████
         ███         
      █████████      
*/
const Buffer sprite_spades =
    (Buffer){.width = 8, .height = 7, .data = (uint8_t[]){0x8, 0x1c, 0x3e, 0x7f, 0x6b, 0x8, 0x1c}};
/*
      ████████████               ████████████      ███                                    ███   
   ██████████████████         ███            ███   ███                                    ███   
████████████████████████      ███               █████████      █████████      ███   ████████████
████████████████████████         ██████            ███      ███         ███   ██████      ███   
████████████████████████               ██████      ███         ████████████   ███         ███   
████████████████████████                     ███   ███      ███         ███   ███         ███   
   ██████████████████         ███            ███   ███      ███      ██████   ███         ███   
      ████████████               ████████████      ██████   █████████   ███   ███         ██████
*/
const Buffer sprite_start = (Buffer){
    .width = 32,
    .height = 8,
    .data = (uint8_t[]){0x3c, 0x78, 0x2,  0x40, 0x7e, 0x84, 0x2,  0x40, 0xff, 0x4,  0xe7,
                        0xf4, 0xff, 0x18, 0x12, 0x4d, 0xff, 0x60, 0xe2, 0x45, 0xff, 0x80,
                        0x12, 0x45, 0x7e, 0x84, 0x92, 0x45, 0x3c, 0x78, 0x76, 0xc5}};
/*
███         ███                     ███                  ███            ████████████               ███                                                                  ███                                       
███         ███                     ███                  ███         ██████████████████            ███                                                                  ███                                       
███         ███      █████████      ███         ████████████      ████████████████████████      █████████      █████████                  ████████████   █████████      ███      ███            ███   █████████   
███████████████   ███         ███   ███      ███         ███      ████████████████████████         ███      ███         ███            ███            ███         ███   ███         ███      ███   ███         ███
███         ███   ███         ███   ███      ███         ███      ████████████████████████         ███      ███         ███               █████████   ███         ███   ███         ███      ███   ███████████████
███         ███   ███         ███   ███      ███         ███         ██████████████████            ███      ███         ███                        ██████         ███   ███            ██████      ███            
███         ███      █████████      ███         ████████████            ████████████               ██████      █████████               ████████████      █████████      ███            ██████         ████████████
*/
const Buffer sprite_solve = (Buffer){
    .width = 72,
    .height = 7,
    .data = (uint8_t[]){0x11, 0x10, 0x8,  0xf,  0x2,  0x0,  0x0,  0x1,  0x0,  0x11, 0x10,
                        0x88, 0x1f, 0x2,  0x0,  0x0,  0x1,  0x0,  0x91, 0x13, 0xcf, 0x3f,
                        0xe7, 0xc0, 0x3b, 0x9,  0x1d, 0x5f, 0x94, 0xc8, 0x3f, 0x12, 0x21,
                        0x44, 0x91, 0x22, 0x51, 0x94, 0xc8, 0x3f, 0x12, 0xc1, 0x45, 0x91,
                        0x3e, 0x51, 0x94, 0x88, 0x1f, 0x12, 0x1,  0x46, 0x61, 0x2,  0x91,
                        0x13, 0xf,  0xf,  0xe6, 0xe0, 0x39, 0x61, 0x3c}};
/*
      █████████      
      █████████      
██████   ███   ██████
█████████████████████
██████   ███   ██████
         ███         
      █████████      
*/
const Buffer sprite_clubs =
    (Buffer){.width = 8, .height = 7, .data = (uint8_t[]){0x1c, 0x1c, 0x6b, 0x7f, 0x6b, 0x8, 0x1c}};
/*
██████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████
███                                                                                                                                                                                                            ███
███                                                                                                                                                                                                            ███
███               ███████████████                                       ██████         ██████            ███                                    ██████                                                         ███
███            █████████████████████                                    ██████         ██████         ██████                                    ██████                                                         ███
███         █████████         █████████                                 ██████         ██████         ██████                                    ██████                                                         ███
███         ██████               ██████                                 ██████                        ██████                                                                                                   ███
███         ██████                              ████████████            ██████         ██████      ███████████████      ███████████████         ██████         ██████   ██████         █████████               ███
███         █████████                        ██████████████████         ██████         ██████      ███████████████      ██████████████████      ██████         ███████████████      ███████████████            ███
███            ███████████████            █████████      █████████      ██████         ██████         ██████         ██████         ██████      ██████         ██████            ██████         ██████         ███
███                  ███████████████      ██████            ██████      ██████         ██████         ██████                        ██████      ██████         ██████            ██████         ██████         ███
███                           █████████   ██████            ██████      ██████         ██████         ██████                  ████████████      ██████         ██████            █████████████████████         ███
███         ██████               ██████   ██████            ██████      ██████         ██████         ██████            ██████████████████      ██████         ██████            █████████████████████         ███
███         ██████               ██████   ██████            ██████      ██████         ██████         ██████         █████████      ██████      ██████         ██████            ██████                        ███
███         █████████         █████████   █████████      █████████      ██████         ██████         ██████         ██████         ██████      ██████         ██████            ██████         ██████         ███
███            █████████████████████         ██████████████████         ██████         ██████         ████████████   █████████████████████      ██████         ██████               ███████████████            ███
███               ███████████████               ████████████            ██████         ██████            █████████      █████████   ██████      ██████         ██████                  █████████               ███
███                                                                                                                                                                                                            ███
███                                                                                                                                                                                                            ███
██████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████
                                                                                                      ███                                                                                          ███            
      ███         ███               ███                     ███                  █████████            ███                                                                        ███               ███            
   ███   ███   ███   ███         ███   ███               █████████               █████████            ███   ███                        █████████                              ███                  ███            
   ███      ███      ███      ███         ███         ███████████████      ██████   ███   ██████      ███   ██████      ███   ███      ███      ███      ███         ███   █████████   ███   ███   ███            
   ███               ███   ███               ███   █████████████████████   █████████████████████      ███   ███   ███   ███   ███      ███      ███   ███   ███   ███   ███   ███      ███   ███   ███            
      ███         ███         ███         ███      ██████   ███   ██████   ██████   ███   ██████      ███   ███   ███      ███         ███      ███   ███   ███   ███   ███   ███         ███      ███            
         ███   ███               ███   ███                  ███                     ███               ███   ██████         ███         █████████         ███         ███      ███         ███      ███            
            ███                     ███                  █████████               █████████            ███                                                                                          ███            
                                                                                                      ████████████████████████████████████████████████████████████████████████████████████████████████            
*/
const Buffer sprite_logo = (Buffer){
    .width = 72,
    .height = 29,
    .data = (uint8_t[]){
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 0x1,  0x0,  0x0,  0x0,  0x0,  0x0,
        0x0,  0x0,  0x20, 0x1,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x20, 0xc1, 0x7,  0x0,
        0x63, 0x8,  0x0,  0x3,  0x0,  0x20, 0xe1, 0xf,  0x0,  0x63, 0xc,  0x0,  0x3,  0x0,  0x20,
        0x71, 0x1c, 0x0,  0x63, 0xc,  0x0,  0x3,  0x0,  0x20, 0x31, 0x18, 0x0,  0x3,  0xc,  0x0,
        0x0,  0x0,  0x20, 0x31, 0x0,  0xf,  0x63, 0x3e, 0x1f, 0x63, 0xe3, 0x20, 0x71, 0x80, 0x1f,
        0x63, 0x3e, 0x3f, 0xe3, 0xf3, 0x21, 0xe1, 0xc3, 0x39, 0x63, 0x8c, 0x31, 0x63, 0x18, 0x23,
        0x81, 0xcf, 0x30, 0x63, 0xc,  0x30, 0x63, 0x18, 0x23, 0x1,  0xdc, 0x30, 0x63, 0xc,  0x3c,
        0x63, 0xf8, 0x23, 0x31, 0xd8, 0x30, 0x63, 0xc,  0x3f, 0x63, 0xf8, 0x23, 0x31, 0xd8, 0x30,
        0x63, 0x8c, 0x33, 0x63, 0x18, 0x20, 0x71, 0xdc, 0x39, 0x63, 0x8c, 0x31, 0x63, 0x18, 0x23,
        0xe1, 0x8f, 0x1f, 0x63, 0xbc, 0x3f, 0x63, 0xf0, 0x21, 0xc1, 0x7,  0xf,  0x63, 0x38, 0x37,
        0x63, 0xe0, 0x20, 0x1,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x20, 0x1,  0x0,  0x0,
        0x0,  0x0,  0x0,  0x0,  0x0,  0x20, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f,
        0x0,  0x0,  0x0,  0x0,  0x4,  0x0,  0x0,  0x0,  0x2,  0x44, 0x10, 0x10, 0x38, 0x4,  0x0,
        0x0,  0x8,  0x2,  0xaa, 0x28, 0x38, 0x38, 0x14, 0xe0, 0x0,  0x4,  0x2,  0x92, 0x44, 0x7c,
        0xd6, 0x34, 0x25, 0x89, 0xae, 0x2,  0x82, 0x82, 0xfe, 0xfe, 0x54, 0x25, 0x55, 0xa5, 0x2,
        0x44, 0x44, 0xd6, 0xd6, 0x54, 0x22, 0x55, 0x45, 0x2,  0x28, 0x28, 0x10, 0x10, 0x34, 0xe2,
        0x88, 0x44, 0x2,  0x10, 0x10, 0x38, 0x38, 0x4,  0x0,  0x0,  0x0,  0x2,  0x0,  0x0,  0x0,
        0x0,  0xfc, 0xff, 0xff, 0xff, 0x3}};
