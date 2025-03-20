#include <future>
#include <iostream>

int compute(int i) { 
    std::cout<<"son thread"<<i<<" id:"<<_threadid<<'\n';
    _sleep(500);
    // sleep(500);
    return i;
 }

int testfuture() {

    std::vector<std::future<int>> results;
    std::cout<<"parent thread id of first time:"<<_threadid<<'\n';
    for (size_t i = 0; i < 50; i++)
    {
        std::future<int> fut = std::async(std::launch::async, compute,i);
        results.emplace_back(std::move(fut));
    }
    
    std::cout<<"parent thread id of second time:"<<_threadid<<'\n';
    for(auto& fuItem:results)
    {
        std::cout << "Result: " << fuItem.get()<<'\n'; // 阻塞直至结果就绪
    }
    std::cout<<"parent thread id of third time:"<<_threadid<<'\n';
    return 0;
}

int testpakaged_task()
{
    /*
    作用：创建一个包装了可调用对象的 std::packaged_task。
    模板参数：int() 表示该任务是一个无参数且返回 int 的函数。
    参数：Lambda 表达式 []{ return 7; } 是被包装的任务，执行时会返回 7。
    特性：std::packaged_task 允许将任务与一个 std::future 绑定，便于异步获取结果
    */
    std::packaged_task<int()> task([]{ return 7; });
    /*
    作用：通过 get_future() 方法获取与 task 关联的 std::future 对象。
    关键点：fut 将用于在后续代码中获取异步任务的结果。
    所有权：std::future 是独占的（不可复制，只能移动）。
    */
    std::future<int> fut = task.get_future();
    /*
    std::move(task)：由于 std::packaged_task 不可复制，必须通过移动语义将 task 转移到线程中。移动后，原始的 task 对象变为无效。
    std::thread 构造：创建一个新线程，执行 std::packaged_task 中包装的任务（即 return 7）。
    detach()：将线程与主线程分离，使其在后台独立运行。主线程无需等待该线程结束。
    */
    std::thread(std::move(task)).detach();
    /*
    fut.get()：阻塞当前线程（主线程），直到异步任务完成并返回结果 7。
    单次有效性：get() 只能调用一次，调用后 fut 变为无效状态。
    异常处理：若任务中抛出异常，get() 会将异常传递到当前线程。
    */
    std::cout << "Result: " << fut.get();   
    return 0;
}

int test_promise()
{
    /*
    作用：创建一个 std::promise<int> 对象 prom，用于在异步操作中存储结果（或异常）。
    核心功能：std::promise 允许显式设置值（set_value）或异常（set_exception），并关联一个 std::future 来获取结果。
    */
    std::promise<int> prom;
    /*
    作用：通过 prom.get_future() 获取与 prom 关联的 std::future 对象 fut。
    特性：fut 用于在后续代码中等待并获取 prom 设置的值或异常。
    所有权：std::future 是独占的，不可复制，只能移动。
    */
    std::future<int> fut = prom.get_future();
    /*
    线程构造：创建一个新线程，执行 Lambda 表达式 [](std::promise<int> p){ p.set_value(9); }。
    参数传递：
    std::move(prom)：将 prom 移动到线程的 Lambda 参数 p 中（std::promise 不可复制，必须使用移动语义）。
    移动后，原始的 prom 对象变为无效，不再可用。
    Lambda 内部操作：
    p.set_value(9)：在新线程中调用 set_value(9)，将值 9 存入 p 关联的 future。
    这一步会唤醒所有等待该 future 的线程（如主线程中的 fut.get()）。
    detach()：将线程与主线程分离，使其在后台独立运行。
    */
    std::thread([](std::promise<int> p){ p.set_value(9); }, std::move(prom)).detach();
    /*
    fut.get()：阻塞当前线程（主线程），直到 prom 被设置值（这里是 9）。
    结果输出：打印 Result: 9。
    单次有效性：get() 只能调用一次，调用后 fut 变为无效状态。
    异常处理：如果 prom 未设置值或设置了异常，get() 会抛出异常。
    */
    std::cout << "Result: " << fut.get();
    return 0;
}

void longTask() {
    std::cout << "Starting long task..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "Long task completed." << std::endl;
}
int testlaunch()
{
    // 使用 std::launch::async 立即启动线程
    std::future<void> fut1 = std::async(std::launch::async, longTask);
    std::cout << "Main thread continues..." << std::endl;
    fut1.get();

    // 使用 std::launch::deferred 延迟执行
    std::future<void> fut2 = std::async(std::launch::deferred, longTask);
    std::cout << "Main thread continues..." << std::endl;

    // 检查任务状态
    std::future_status status = fut2.wait_for(std::chrono::milliseconds(0));
    if (status == std::future_status::deferred) {
        std::cout << "Task has not started yet." << std::endl;
    }

    // 获取结果以启动任务
    fut2.get();

    return 0;
}