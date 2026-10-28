use crate::vector::MyVec;

pub struct Stack<T> {
    data: MyVec<T>,
}

impl<T> Stack<T> {
    pub fn new() -> Self {
        Self {
            data: MyVec::new(),
        }
    }

    pub fn with_capacity(capacity: usize) -> Self {
        Self {
            data: MyVec::with_capacity(capacity),
        }
    }

    pub fn push(&mut self, val: T) {
        self.data.push(val);
    }

    pub fn pop(&mut self) -> Option<T> {
        self.data.pop()
    }

    pub fn peek(&self) -> Option<&T> {
        if self.data.is_empty() {
            None
        } else {
            self.data.get(self.data.len() - 1)
        }
    }

    pub fn peek_mut(&mut self) -> Option<&mut T> {
        if self.data.is_empty() {
            None
        } else {
            let last_idx = self.data.len() - 1;
            self.data.get_mut(last_idx)
        }
    }

    pub fn len(&self) -> usize {
        self.data.len()
    }

    pub fn is_empty(&self) -> bool {
        self.data.is_empty()
    }

    pub fn clear(&mut self) {
        self.data.clear();
    }
}

impl<T> Default for Stack<T> {
    fn default() -> Self {
        Self::new()
    }
}
