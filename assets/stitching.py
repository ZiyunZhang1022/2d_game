import os
import re
from PIL import Image


# Reformatting Source Code: Ctrl+Alt+L
class ImageGroup():
    def __init__(self, dir, group, image):
        self.dir = dir
        self.group = group
        self.image_names = [image]
        self.images = []

    def append(self, image):
        self.image_names.append(image)

    def image_stitching(self):
        for image_name in self.image_names:
            self.images.append(Image.open(dir + image_name))
        (width, height) = self.images[0].size
        result = Image.new("RGBA", (width * len(self.images), height))
        for i, image in enumerate(self.images):
            if not image.size == (width, height):
                image = image.resize((width, height))
            result.paste(image, ((i * width, 0, (i + 1) * width, height)))
        result.save(self.dir + self.group + ".png")
        return result


if __name__ == '__main__':
    dir = './'
    if not os.path.isdir(dir):
        print("Not a directory!")
        exit()
    else:
        print("Directory: %s" % (dir))

    files = os.listdir(dir)

    name = r'\w+'
    numbers = r'[0-9]+'
    not_numbers = r'[^0-9]'
    png = r'(png|PNG)'
    dot = r'\.'

    pic_list = []  # save image groups
    for file in files:
        if re.match(name + dot + numbers + dot + png, file):
            print(file)
            pic_group = file.split('.')[0]

            if not pic_list:
                pic_list.append(ImageGroup(dir, pic_group, file))

            else:
                has_pic = False
                for pic in pic_list[::-1]:  # reverse traversal
                    if pic.group == pic_group:
                        pic.append(file)
                        # print("has")
                        has_pic = True
                if not has_pic:
                    pic_list.append(ImageGroup(dir, pic_group, file))
                    # print("append")

    print(pic_list)
    for pic in pic_list:
        print(pic.group)
        print(pic.image_names)
        pic.image_stitching()
