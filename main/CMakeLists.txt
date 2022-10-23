# This example code is in the Public Domain (or CC0 licensed, at your option.)
# Unless required by applicable law or agreed to in writing, this
# software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
# CONDITIONS OF ANY KIND, either express or implied.

idf_component_register(
    SRCS
        "main.c"
        "benchsys.c"
        "testing.cpp"
        "mch2022-demo/techdemo.c"
        "mch2022-demo/sponsors.c"
        "mch2022-demo/images/anim_intro.c"
       
    INCLUDE_DIRS
        "." "include" "mch2022-demo"
    EMBED_FILES
        ${project_dir}/main/my_image.png
)
