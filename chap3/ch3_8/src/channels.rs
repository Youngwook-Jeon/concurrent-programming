use std::{sync::{Arc, Mutex, Condvar}, collections::LinkedList};

use crate::semaphore::Semaphore;

const NUM_LOOP: usize = 100000;
const NUM_THREADS: usize = 8;

#[derive(Clone)]
pub struct Sender<T> {
    sem: Arc<Semaphore>,
    buf: Arc<Mutex<LinkedList<T>>>, // 큐
    cond: Arc<Condvar>, // 읽기 측의 조건 변수
}

impl<T: Send> Sender<T> { // Send 트레잇 구현을 요구함으로써 허용된 타입만 송신 가능하게 함.
    pub fn send(&self, data: T) {
        self.sem.wait();
        let mut buf = self.buf.lock().unwrap();
        buf.push_back(data);
        self.cond.notify_one();
    }
}

pub struct Receiver<T> {
    sem: Arc<Semaphore>,
    buf: Arc<Mutex<LinkedList<T>>>, // 큐
    cond: Arc<Condvar>, // 읽기 측의 조건 변수
}

impl<T> Receiver<T> {
    pub fn recv(&self) -> T {
        let mut buf = self.buf.lock().unwrap();
        loop {
            if let Some(data) = buf.pop_front() {
                self.sem.post();
                return data;
            }
            // 빈 경우 대기
            buf = self.cond.wait(buf).unwrap();
        }
    }
}

// Sender, Receiver 생성하기.
pub fn channel<T>(max: isize) -> (Sender<T>, Receiver<T>) {
    assert!(max > 0);
    let sem = Arc::new(Semaphore::new(max));
    let buf = Arc::new(Mutex::new(LinkedList::new()));
    let cond = Arc::new(Condvar::new());
    let tx = Sender {
        sem: sem.clone(),
        buf: buf.clone(),
        cond: cond.clone(),
    };
    let rx = Receiver {sem, buf, cond };
    (tx, rx)
}

pub fn run_channels() {
    let (tx, rx) = channel(4);
    let mut v = Vec::new();

    // 수신용 스레드
    let t = std::thread::spawn(move || {
        let mut cnt = 0;
        while cnt < NUM_THREADS * NUM_LOOP {
            let n = rx.recv();
            println!("recv: n = {:?}", n);
            cnt += 1;
        }
    });

    v.push(t);

    // 송신용 스레드
    for i in 0..NUM_THREADS {
        let tx0 = tx.clone();
        let t = std::thread::spawn(move || {
            for j in 0..NUM_LOOP {
                tx0.send((i, j));
            }
        });
        v.push(t);
    }

    for t in v {
        t.join().unwrap();
    }
}