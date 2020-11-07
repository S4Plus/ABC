# QuantumMachine

关于QPanda量子虚拟机的相关知识可以参考[QPanda提供的文档](https://qpanda-tutorial.readthedocs.io/zh/latest/QuantumMachine.html)。

## 接口

### QCloudMachine

**CLOUD_QMACHINE_TYPE**

```c++
enum class CLOUD_MACHINE_TYPE {
  Full_AMPLITUDE,
  NOISE_QMACHINE,
  PARTIAL_AMPLITUDE,
  SINGLE_AMPLITUDE,
  CHEMISTRY,
  REAL_CHIP
};
```

简介：枚举类型，定义量子云计算机类型，具体指哪一种类型，可以参考官方文档。其中CHEMISTRY没有找到对应哪一种虚拟机

**REAL_CHIP_TYPE**

```c++
enum class REAL_CHIP_TYPE {
  ORIGIN_WUYUAN
};
```

简介：从名称和内容上看，包含的是提供的真实芯片名。目前只有提供悟源芯片

**NoiseParams**

```c++
struct NoiseParams {
  string noise_model;
  double single_gate_param;
  double double_gate_param;
  
  double single_p2;
  double double_p2;
  
  double single_pgate;
  double double_pgate;
};
```

该结构在`QCloudMachine`类中使用，保存云计算机的噪声信息

**QCloudMachine**

用于连接量子云端服务器的量子云机器类型，属于量子计算机部分

```c++
class QCloudMachine: public QVM{
public:
  QCloudMachine();
  ~QCloudMachine();
  void init();
  void init(string token);
  
  void set_compute_api(std::string url) { m_compute_url = url; }
  void set_inqure_api(std::string url) { m_inqure_api = url; }
  
  void set_noise_model(NOISE_MODEL model, const std::vector<double> single_params, const std::vector<double> double_params);
  
  std::map<std::string, double> noise_measure(QProg &, int shot, string task_name = "Qurator Experiment");
  std::map<std::string, double> full_amplitude_measure(QProg &, int shot, string task_name = "Qurator Experiment");
  std::map<std::string, double> full_amplitude_pmeasure(QProg &prog, Qnum qubit_vec, string task_name = "Qurator Experiment")
  std::map<std::string, qcomplex_t> partial_amplitude_pmeasure(QProg &prog, std::vector<std::string> amplitude_vec, string task_name = "Qurator Experiment");
	qcomplex_t single_amplitude_pmeasure(QProg &prog, std::string amplitude, string task_name = "Qurator Experiment");
	std::map<std::string, double> real_chip_measure(QProg &, int shot, string task_name = "Qurator Experiment", REAL_CHIP_TYPE type = REAL_CHIP_TYPE::ORIGIN_WUYUAN);
	std::vector<QStat> get_state_tomography_density(QProg &, int shot);
	std::string get_result_json(std::string taskid, CLOUD_QMACHINE_TYPE type);
private:
	std::string m_token;
	std::string m_inqure_url;
  std::string m_compute_url;

  std::map<std::string, double> m_measure_result;
  std::map<std::string, qcomplex_t> m_pmeasure_result;
  std::vector<QStat> m_qst_result;
  qcomplex_t m_single_result;
  NoiseParams m_noise_params;

  enum CLUSTER_TASK_TYPE
  {
      CLUSTER_MEASURE = 1,
      CLUSTER_PMEASURE
  };

  enum class TASK_STATUS
  {
  	WAITING = 1, 
    COMPUTING,
    FINISHED,
    FAILED,
    QUEUING,

    //The next status only appear in real chip backend
    SENT_TO_BUILD_SYSTEM,
    BUILD_SYSTEM_ERROR,   
    SEQUENCE_TOO_LONG,
    BUILD_SYSTEM_RUN
  };

  std::string post_json(const std::string &, std::string &);

  void inqure_result(std::string, CLOUD_QMACHINE_TYPE);

  bool parser_cluster_result_json(std::string &recv_json, std::string&);
  bool parser_cluster_submit_json(std::string &recv_json, std::string&);

	void add_string_value(rapidjson::Document &, const string &, const size_t);
	void add_string_value(rapidjson::Document &, const string &, const double);
	void add_string_value(rapidjson::Document &, const string &, const std::string &);
}
```



## 量子云计算执行分析

​	这一部分讨论的是在本源的量子云平台上进行计算的流程。目前本源提供的真实芯片(REAL_CHIP_TYPE)只有悟源芯片一种，执行方式是调用`QCloudMachine`类的`real_chip_measure`接口提交本地任务。

​	通过量子云平台向本源真实芯片提供计算任务的代码可以参考QPanda[量子云虚拟机](https://qpanda-tutorial.readthedocs.io/zh/latest/QCloudMachine.html)文档。调用上可以概括为：

```c++
auto result = QCM.real_chip_measure(measure_prog, 100);
```

​	`real_chip_measure`的函数定义为：

```c++
	std::map<std::string, double> real_chip_measure(QProg &, int shot, string task_name = "Qurator Experiment", REAL_CHIP_TYPE type = REAL_CHIP_TYPE::ORIGIN_WUYUAN);
```

​	其中第一个参数是需要上传到云平台执行的量子程序，第二参数`shot`为需要执行的次数。第三参数为`task_name`，这与任务在控制台显示的名字相关。第四个参数`type`指定的是运行的芯片，目前只有悟源芯片，故默认值为`READ_CHIP_TYPE::ORIGIN_WUYUAN`。

​	从函数的整体看，`real_chip_measure`函数利用传入的量子程序和`shot`值，将其和其他信息打包成一个json文件发送给量子云平台。其流程如下：

```c++
//convert prog to originir 
auto prog_str = convert_qprog_to_originir(prog, this);

//construct json
rapidjson::Document doc;
doc.SetObject();

add_string_value(doc, "code", prog_str);
add_string_value(doc, "apiKey", m_token);
add_string_value(doc, "QMachineType", (size_t)CLOUD_QMACHINE_TYPE::REAL_CHIP);
add_string_value(doc, "codeLen", prog_str.size());
add_string_value(doc, "qubitNum", "6");
add_string_value(doc, "measureType", (size_t)CLUSTER_TASK_TYPE::CLUSTER_MEASURE);
add_string_value(doc, "classicalbitNum", getAllocateCMem());
add_string_value(doc, "shot", (size_t)shots);
add_string_value(doc, "taskName", task_name); 

StringBuffer buffer;
Writer<StringBuffer> writer(buffer);
doc.Accept(writer);

std::string post_json_str = buffer.GetString();
std::string recv_json_str = post_json(m_compute_url, post_json_str);

inqure_result(recv_json_str, CLOUD_QMACHINE_TYPE::REAL_CHIP);
```

​	在json文件中包括的信息由OriginIR代码， 用户token，量子机器类型（真实芯片）、代码长度、量子比特数、测量类型（集群测量）、经典量子比特数、测试次数和任务名。

​	在将量子程序的信息放入json文件前，函数还需要将其转化成`OriginIR`格式。虽然不了解`StringBuffer`和`Writer`之间的具体操作不太，但是从函数的功能上来看应该是将json文件中的信息传入到`buffer`当中。在经过一系列操作之后，`buffer`中的信息有传入到了`recv_json_str`中。之后，`post_json`函数利用传入的url以及json文件进行发送。

​	其中`post_json`函数调用了libcurl库实现通信。

​	最后调用了`inqure_result`函数。从名称上看作用为查询结果：

```c++
void QCloudMachine::inqure_result(std::string recv_json_str, CLOUD_QMACHINE_TYPE type)
{
    std::string taskid;
    if (parser_cluster_submit_json(recv_json_str, taskid))
    {
        bool retry_inquire = false;
        do
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(3000));
            auto result_json = get_result_json(taskid, type);
            retry_inquire = parser_cluster_result_json(result_json, taskid);
        } while (retry_inquire);
    }
    return;
}
```

​	从函数的整体上看，该函数的作用为将任务提交后反复检查任务是否完成。



