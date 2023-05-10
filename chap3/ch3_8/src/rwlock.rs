use std::sync::RwLock;

pub fn run_rwlock() {
    let lock = RwLock::new(10); // RW락용 값 생성, 보호 대상의 초깃값인 10 지정.
    {
        let v1 = lock.read().unwrap();
        let v2 = lock.read().unwrap();
        println!("v1 = {}", v1);
        println!("v2 = {}", v2);
    } // 스코프를 벗어나면 자동으로 Read락 해제.

    {
        let mut v = lock.write().unwrap();
        *v = 7;
        println!("v = {}", v);
    }
}