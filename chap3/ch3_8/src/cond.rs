use std::sync::{Arc, Mutex, Condvar};
use std::thread;

// Condvar: 조건 변수
fn child(id: u64, p: Arc<(Mutex<bool>, Condvar)>) {
    let &(ref lock, ref cvar) = &*p;

    let mut started = lock.lock().unwrap();
    while !*started { //Mutex 안의 공유 변수가 false인 동안 루프돌기
        started = cvar.wait(started).unwrap();
    }

    // 다음과 같이 wait_while을 사용할 수도 있음
    // cvar.wait_while(started, |started| !*started).unwrap();
    println!("child {}", id);
}

fn parent(p: Arc<(Mutex<bool>, Condvar)>) {
    let &(ref lock, ref cvar) = &*p;

    let mut started = lock.lock().unwrap();
    *started = true;
    cvar.notify_all();
    println!("parent");
}

pub fn run_cond() {
    let pair0 = Arc::new((Mutex::new(false), Condvar::new()));
    let pair1 = pair0.clone();
    let pair2 = pair0.clone();

    let c0 = thread::spawn(move || { child(0, pair0) });
    let c1 = thread::spawn(move || { child(1, pair1) });
    let p = thread::spawn(move || { parent(pair2) });

    c0.join().unwrap();
    c1.join().unwrap();
    p.join().unwrap();
}
