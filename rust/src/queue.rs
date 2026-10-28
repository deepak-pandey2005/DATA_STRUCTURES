pub struct Queue<T> {
    buf: Vec<Option<T>>,
    head: usize,
    tail: usize,
    len: usize,
}

impl<T> Queue<T> {
    pub fn new() -> Self {
        Self::with_capacity(8)
    }

    pub fn with_capacity(capacity: usize) -> Self {
        let cap = if capacity == 0 { 8 } else { capacity };
        let mut buf = Vec::with_capacity(cap);
        for _ in 0..cap {
            buf.push(None);
        }
        Self {
            buf,
            head: 0,
            tail: 0,
            len: 0,
        }
    }

    pub fn len(&self) -> usize {
        self.len
    }

    pub fn is_empty(&self) -> bool {
        self.len == 0
    }

    pub fn enqueue(&mut self, val: T) {
        if self.len == self.buf.len() {
            self.grow();
        }

        self.buf[self.tail] = Some(val);
        self.tail = (self.tail + 1) % self.buf.len();
        self.len += 1;
    }

    pub fn dequeue(&mut self) -> Option<T> {
        if self.len == 0 {
            return None;
        }

        let val = self.buf[self.head].take();
        self.head = (self.head + 1) % self.buf.len();
        self.len -= 1;
        val
    }

    pub fn peek(&self) -> Option<&T> {
        if self.len == 0 {
            None
        } else {
            self.buf[self.head].as_ref()
        }
    }

    fn grow(&mut self) {
        let old_cap = self.buf.len();
        let new_cap = old_cap * 2;
        let mut new_buf = Vec::with_capacity(new_cap);

        for i in 0..self.len {
            let idx = (self.head + i) % old_cap;
            new_buf.push(self.buf[idx].take());
        }
        for _ in self.len..new_cap {
            new_buf.push(None);
        }

        self.buf = new_buf;
        self.head = 0;
        self.tail = self.len;
    }

    pub fn clear(&mut self) {
        while self.dequeue().is_some() {}
    }
}

impl<T> Default for Queue<T> {
    fn default() -> Self {
        Self::new()
    }
}
