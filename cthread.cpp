
#include "main.h"
#include "cthread.h"

CThread::CThread() {
}

CThread::~CThread() {
}

void CThread::InitPThreads() {
#ifdef WIN32
	pthread_win32_process_attach_np();
	pthread_win32_thread_attach_np();
#else
#endif // WIN32
}

void CThread::CleanupPThreads() {
#ifdef WIN32
	pthread_win32_process_detach_np();
	pthread_win32_thread_detach_np();
#else
#endif // WIN32
}

bool CThread::Start() {
	if(!pthread_create(&m_tThread, NULL, &this->RunWrapper, this)) return true;
	return false;
}

void *CThread::Join() {
	void *pRetVal;
	pthread_join(m_tThread, &pRetVal);
	return pRetVal;
}

void CThread::Kill() {
	pthread_kill(m_tThread, SIGTERM);
}

pthread_t CThread::GetThread() {
	return m_tThread;
}

void *CThread::RunWrapper(void *pArgs) {
	CThread *pThread=(CThread*)pArgs;
	pThread->Run();
	return NULL;
}
