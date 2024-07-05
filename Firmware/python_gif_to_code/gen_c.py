import gif2numpy
import pdb
import cv2
import pandas 
import numpy
import pysvg.structure
import pysvg.builders
import pysvg.text


#  np_frames, extensions, image_specifications = gif2numpy.convert("rainbow.gif")
np_frames, extensions, image_specifications = gif2numpy.convert("rickroll.gif")
#  np_frames, extensions, image_specifications = gif2numpy.convert("bear.gif")
#  np_frames, extensions, image_specifications = gif2numpy.convert("scrolling_rainbow.gif")
#print(led_locs)


#print(image_specifications)
gif_max_x = image_specifications["Image Size"][0]
gif_max_y = image_specifications["Image Size"][1]

# for led in led_locs:
#     if led[3] < led_min_x:
#         led_min_x = led[3]
#     if led[4] < led_min_y:
#         led_min_y = led[4]
#     if led[3] > led_max_x:
#         led_max_x = led[3]
#     if led[4] > led_max_y:
#         led_max_y = led[4]
#print(f"LED Max: {led_max_x}, {led_max_y} , LED Min: {led_min_x}, {led_min_y}")
# print(f"pub static frames : [[[u8;3];512];{len(np_frames)}]= [")
out_frames = []
for i, frame in enumerate(np_frames):
    out_frame = []
    svg_document = pysvg.structure.Svg()
    shape_builder = pysvg.builders.ShapeBuilder()
    for row in range(32):
        out_frame_row = []
        for col in range(32):
            pixel_x = int(numpy.interp(col, (0, 31), (0, gif_max_x-1)))
            pixel_y = int(numpy.interp(row, (0, 31), (0, gif_max_y-1)))
            led_red = int(frame[pixel_y][pixel_x][2])
            led_green = int(frame[pixel_y][pixel_x][1])
            led_blue = int(frame[pixel_y][pixel_x][0])
            # print(led_red)
            r,g,b,w = 0,0,0,0
            if ((led_red+led_blue+led_green)/3) > 128:
                w=1
            if led_red > 64:
                r = 1
            if led_green > 64:
                g = 1
            if led_blue > 64:
                b = 1
            # frame[[row,col]] = [r,g,b,w]
            out_frame_row.append([r,g,b,w])
            svg_document.addElement(shape_builder.createCircle(pixel_x, pixel_y, 5, fill=f"rgb({r}, {g}, {b})"))

        out_frame.append(out_frame_row)
    out_frames.append(out_frame)

header = ""
num_bytes = 0
total_num_bytes = 0
num_bytes_per_frame = 64 * 64 /8
num_frames = len(out_frames)
svg_document.save(f"gif/frame{i:02}.svg")
for frame in out_frames:
    for row_b in range(64):
        row_bits = ""
        cur_col_bits = ""
        for col_b in range(64):
            row_i = int((row_b/2))
            col_i = int((col_b/2))
            if row_b % 2:
                #red green
                if col_b % 2:
                    #red
                    cur_col_bits += str(frame[row_i][col_i][0])
                else:
                    cur_col_bits += str(frame[row_i][col_i][1])
                    #green
            else:
                #blue white
                if col_b % 2:
                    cur_col_bits += str(frame[row_i][col_i][2])
                    # blue
                else:
                    cur_col_bits += str(frame[row_i][col_i][3])
                    #white
            if ((col_b + 1) % 8 == 0):
                row_bits += hex(int(cur_col_bits,2)) + ", "
                num_bytes += 1
                total_num_bytes += 1
                cur_col_bits = ""
        header += row_bits  + "\n"
    header += "// New frame\n"

# typedef uint8_t frame[""" + str(int(num_bytes_per_frame)) + "];" + """
header = """
#include <stdint.h>

int num_frames = """ + str(int(num_frames)) + """;

    typedef struct frame { uint8_t x[""" + str(int(num_bytes_per_frame)) + """]; } frame;

typedef union Frames{
        frame frames[""" + str(num_frames) + """];
        uint8_t bytes[""" + str(total_num_bytes) + """];
    } Frames;
Frames frames = {
        """ + header + "\n };"

print(header)

        


