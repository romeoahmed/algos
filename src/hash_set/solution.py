from enum import Enum

INITIAL_CAPACITY = 8  # Every capacity is a power of two.
GOLDEN = 0x9E3779B97F4A7C15  # Odd integer just below 2**64 / golden ratio.


def home(key: int, capacity: int) -> int:
    """Map key to range(capacity) by Fibonacci hashing.

    Multiply the hash by 2**64 / golden ratio modulo 2**64 and keep the top
    log2(capacity) bits, so runs of consecutive keys or keys sharing low bits
    still spread over the table. hash(key) % capacity would keep only the low
    bits of Python's identity-like integer hash.
    """
    bits = capacity.bit_length() - 1
    mixed = (hash(key) * GOLDEN) & ((1 << 64) - 1)
    return mixed >> (64 - bits)


class ChainedHashSet:
    """Integer set using separate chaining.

    Doubles the bucket count whenever the size would exceed it, so chains
    average at most one key.
    """

    def __init__(self) -> None:
        self._buckets: list[list[int]] = [[] for _ in range(INITIAL_CAPACITY)]
        self._size = 0

    def _chain(self, key: int) -> list[int]:
        return self._buckets[home(key, len(self._buckets))]

    def __contains__(self, key: int) -> bool:
        return key in self._chain(key)

    def __len__(self) -> int:
        return self._size

    def add(self, key: int) -> bool:
        """Insert key; return whether the set changed."""
        chain = self._chain(key)
        if key in chain:
            return False
        chain.append(key)
        self._size += 1
        if self._size > len(self._buckets):
            self._rehash(2 * len(self._buckets))
        return True

    def remove(self, key: int) -> bool:
        """Remove key; return whether the set changed."""
        chain = self._chain(key)
        if key not in chain:
            return False
        chain.remove(key)
        self._size -= 1
        return True

    def _rehash(self, capacity: int) -> None:
        keys = [key for chain in self._buckets for key in chain]
        self._buckets = [[] for _ in range(capacity)]
        for key in keys:
            self._chain(key).append(key)


class Mark(Enum):
    """Slot states besides holding a key."""

    EMPTY = "empty"  # Never used: probe sequences stop here.
    DELETED = "deleted"  # Tombstone: probe sequences pass through.


class ProbingHashSet:
    """Integer set using linear probing with tombstones.

    Rebuilds once keys plus tombstones would fill more than 3/4 of the slots,
    so every probe sequence reaches an EMPTY slot.
    """

    def __init__(self) -> None:
        self._slots: list[int | Mark] = [Mark.EMPTY] * INITIAL_CAPACITY
        self._size = 0
        self._deleted = 0

    def _locate(self, key: int) -> tuple[int, bool]:
        """Return (slot holding key, True) or (slot where key belongs, False)."""
        capacity = len(self._slots)
        free: int | None = None  # First tombstone on the way.
        index = home(key, capacity)
        while (slot := self._slots[index]) is not Mark.EMPTY:
            if slot is Mark.DELETED:
                free = index if free is None else free
            elif slot == key:
                return index, True
            index = (index + 1) % capacity
        return (index if free is None else free), False

    def __contains__(self, key: int) -> bool:
        return self._locate(key)[1]

    def __len__(self) -> int:
        return self._size

    def add(self, key: int) -> bool:
        """Insert key, reusing the first tombstone; return whether it changed."""
        index, found = self._locate(key)
        if found:
            return False
        if self._slots[index] is Mark.DELETED:
            self._deleted -= 1
        self._slots[index] = key
        self._size += 1
        capacity = len(self._slots)
        if 4 * (self._size + self._deleted) > 3 * capacity:
            # Grow only when live keys need it; otherwise just drop tombstones.
            self._rebuild(2 * capacity if 2 * self._size > capacity else capacity)
        return True

    def remove(self, key: int) -> bool:
        """Leave a tombstone at key; return whether the set changed."""
        index, found = self._locate(key)
        if not found:
            return False
        self._slots[index] = Mark.DELETED
        self._size -= 1
        self._deleted += 1
        return True

    def _rebuild(self, capacity: int) -> None:
        keys = [slot for slot in self._slots if not isinstance(slot, Mark)]
        self._slots = [Mark.EMPTY] * capacity
        self._deleted = 0
        for key in keys:
            self._slots[self._locate(key)[0]] = key
