#include <vtkSmartPointer.h>
#include <vtkRendererCollection.h>
#include <vtkPointPicker.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkActor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkObjectFactory.h>
#include <vtkProperty.h>
#include <vtkSTLReader.h>
#include <vtkCylinderSource.h>
#include <vtkCubeAxesActor.h>
#include <vtkAxesActor.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkAutoInit.h> 
#include <vtkCamera.h>
#include <vtkWindowToImageFilter.h>


using namespace std;

VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType)
int main(int, char* [])
{
	// 读取文件
	vtkSmartPointer<vtkPolyData> input1 = vtkSmartPointer<vtkPolyData>::New();
	vtkSmartPointer<vtkSTLReader> reader1 = vtkSmartPointer<vtkSTLReader>::New();
	reader1->SetFileName("res/111.stl");
	reader1->Update();
	input1->DeepCopy(reader1->GetOutput());

	// 创建映射器
	vtkSmartPointer<vtkPolyDataMapper> mapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(reader1->GetOutputPort());
	vtkSmartPointer<vtkActor> actor =
		vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	actor->GetProperty()->SetOpacity(0.7);

	// 创建渲染器、渲染窗口和交互器
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->Render();
	renderWindow->SetWindowName("PointPicker");
	renderWindow->AddRenderer(renderer);

	vtkSmartPointer<vtkPointPicker> pointPicker = vtkSmartPointer<vtkPointPicker>::New();

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetPicker(pointPicker);
	renderWindowInteractor->SetRenderWindow(renderWindow);

	// Add the actor to the scene
	renderer->AddActor(actor);


	renderer->ResetCamera();
	//开启正交投影模式
	renderer->GetActiveCamera()->ParallelProjectionOn();
	renderer->GetActiveCamera()->SetParallelScale(10);
	renderer->GetActiveCamera()->SetClippingRange(-1000, 1000);
	
	
	renderer->SetBackground(0, 0, 0);

	// Render and interact
	renderWindow->Render();
	//renderWindowInteractor->Start();

	// 将渲染窗口内容转换为图像数据
	vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter = vtkSmartPointer<vtkWindowToImageFilter>::New();
	windowToImageFilter->SetInput(renderWindow);
	windowToImageFilter->Update();

	// 获取图像数据
	vtkSmartPointer<vtkImageData> imageData = windowToImageFilter->GetOutput();

	// 获取图像数据的维度
	int* dims = imageData->GetDimensions();
	int width = dims[0];
	int height = dims[1];

	// 获取图像数据的像素数组
	unsigned char* pixels = static_cast<unsigned char*>(imageData->GetScalarPointer());

	// 创建矩阵数组并填充像素数据
	std::vector<std::vector<unsigned char>> matrixArray(height, std::vector<unsigned char>(width));
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			matrixArray[y][x] = pixels[(y * width + x) * 3]; // 只保存一个通道的数据，这里假设为RGB图像且只保存红色通道
		}
	}

	// 打印矩阵数组的内容（可选）
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			std::cout << static_cast<int>(matrixArray[y][x]) << " ";
		}
		std::cout << std::endl;
	}


	return EXIT_SUCCESS;
}