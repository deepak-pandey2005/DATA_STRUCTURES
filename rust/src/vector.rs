use std::alloc::{alloc, dealloc, realloc, Layout};
use std::ops::{Deref, DerefMut};
use std::ptr::{self, NonNull};

pub struct MyVec<T> {
    ptr: NonNull<T>,
    cap: usize,
    len: usize,
}

unsafe impl<T: Send> Send for MyVec<T> {}
unsafe impl<T: Sync> Sync for MyVec<T> {}

impl<T> MyVec<T> {
    pub fn new() -> Self {
        Self {
            ptr: NonNull::dangling(),
            cap: 0,
            len: 0,
        }
    }

    pub fn with_capacity(capacity: usize) -> Self {
        if capacity == 0 || std::mem::size_of::<T>() == 0 {
            return Self::new();
        }

        let layout = Layout::array::<T>(capacity).expect("Capacity overflow");
        let new_ptr = unsafe { alloc(layout) } as *mut T;
        let ptr = NonNull::new(new_ptr).expect("Out of memory");

        Self {
            ptr,
            cap: capacity,
            len: 0,
        }
    }

    pub fn len(&self) -> usize {
        self.len
    }

    pub fn capacity(&self) -> usize {
        self.cap
    }

    pub fn is_empty(&self) -> bool {
        self.len == 0
    }

    pub fn push(&mut self, elem: T) {
        if self.len == self.cap {
            self.grow();
        }
        unsafe {
            ptr::write(self.ptr.as_ptr().add(self.len), elem);
        }
        self.len += 1;
    }

    pub fn pop(&mut self) -> Option<T> {
        if self.len == 0 {
            None
        } else {
            self.len -= 1;
            unsafe { Some(ptr::read(self.ptr.as_ptr().add(self.len))) }
        }
    }

    pub fn insert(&mut self, index: usize, elem: T) {
        assert!(index <= self.len, "Index out of bounds");
        if self.len == self.cap {
            self.grow();
        }
        unsafe {
            let p = self.ptr.as_ptr().add(index);
            if index < self.len {
                ptr::copy(p, p.add(1), self.len - index);
            }
            ptr::write(p, elem);
        }
        self.len += 1;
    }

    pub fn remove(&mut self, index: usize) -> T {
        assert!(index < self.len, "Index out of bounds");
        unsafe {
            let p = self.ptr.as_ptr().add(index);
            let result = ptr::read(p);
            ptr::copy(p.add(1), p, self.len - index - 1);
            self.len -= 1;
            result
        }
    }

    pub fn get(&self, index: usize) -> Option<&T> {
        if index < self.len {
            unsafe { Some(&*self.ptr.as_ptr().add(index)) }
        } else {
            None
        }
    }

    pub fn get_mut(&mut self, index: usize) -> Option<&mut T> {
        if index < self.len {
            unsafe { Some(&mut *self.ptr.as_ptr().add(index)) }
        } else {
            None
        }
    }

    pub fn clear(&mut self) {
        while self.pop().is_some() {}
    }

    fn grow(&mut self) {
        if std::mem::size_of::<T>() == 0 {
            self.cap = usize::MAX;
            return;
        }

        let new_cap = if self.cap == 0 { 8 } else { self.cap * 2 };
        let new_layout = Layout::array::<T>(new_cap).expect("Capacity overflow");

        let new_ptr = if self.cap == 0 {
            unsafe { alloc(new_layout) }
        } else {
            let old_layout = Layout::array::<T>(self.cap).unwrap();
            unsafe { realloc(self.ptr.as_ptr() as *mut u8, old_layout, new_layout.size()) }
        } as *mut T;

        self.ptr = NonNull::new(new_ptr).expect("Out of memory");
        self.cap = new_cap;
    }
}

impl<T> Default for MyVec<T> {
    fn default() -> Self {
        Self::new()
    }
}

impl<T> Deref for MyVec<T> {
    type Target = [T];
    fn deref(&self) -> &[T] {
        unsafe { std::slice::from_raw_parts(self.ptr.as_ptr(), self.len) }
    }
}

impl<T> DerefMut for MyVec<T> {
    fn deref_mut(&mut self) -> &mut [T] {
        unsafe { std::slice::from_raw_parts_mut(self.ptr.as_ptr(), self.len) }
    }
}

impl<T> Drop for MyVec<T> {
    fn drop(&mut self) {
        self.clear();
        if self.cap != 0 && std::mem::size_of::<T>() != 0 {
            let layout = Layout::array::<T>(self.cap).unwrap();
            unsafe {
                dealloc(self.ptr.as_ptr() as *mut u8, layout);
            }
        }
    }
}
