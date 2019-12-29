CC=g++
FLAGS=-mwindows -static -DNDEBUG -O3
EXE=BingDesktop.exe
OBJS=main.o GLShader.o GLRenderObject.o openCVImageFrame.o preReadFile.o common.o utilsTool.o openCVVedioFrame.o \
	fileDlg.o systemNotify.o
RES=res.res

start:$(OBJS) $(RES)
	$(CC) $(FLAGS) -o $(EXE) $(OBJS) $(RES) \
	-l libglfw3 \
	-l glew32 \
	-l opengl32 \
	-l libopencv_core411 \
	-l libopencv_highgui411 \
	-l libopencv_imgproc411 \
	-l libopencv_video411 \
	-l libopencv_calib3d411 \
	-l libopencv_dnn411 \
	-l libopencv_features2d411 \
	-l libopencv_flann411 \
	-l libopencv_gapi411 \
	-l libopencv_imgcodecs411 \
	-l libopencv_ml411 \
	-l libopencv_objdetect411 \
	-l libopencv_photo411 \
	-l libopencv_stitching411 \
	-l libopencv_videoio411 -l gdi32 -l comdlg32
	
	
$(filter %.o, $(OBJS)):%.o:%.cpp
	$(CC) $(FLAGS) -c $< -o $@
$(filter %.res, $(RES)):%.res:%.rc
	windres --input-format=rc --output-format=coff -i $< -o $@
.PHONY:clean
clean:
	-rm -rf $(OBJS);rm -rf $(RES);rm -rf $(EXE);
