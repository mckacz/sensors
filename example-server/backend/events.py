from threading import Lock


class Dispatcher:
    """
    Simple event dispatcher.
    """

    _lock = Lock()
    _handlers = dict()

    def on(self, event: str, handler):
        self._lock.acquire()

        if event not in self._handlers:
            self._handlers[event] = []

        self._handlers[event].append(handler)
        self._lock.release()

    def off(self, event: str, handler):
        self._lock.acquire()

        if event in self._handlers:
            self._handlers[event].remove(handler)

        self._lock.release()

    def trigger(self, event, sender: object, **kwargs):
        self._lock.acquire()

        if event in self._handlers:
            for handler in self._handlers[event]:
                handler(event, sender, **kwargs)

        self._lock.release()


