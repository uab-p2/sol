#pragma once
#include <vector>

// Auxiliary classes used in some of the advanced skills (see below)
class IntPredicate;
class IntOperation;

/// A toolbox of `vector<int>` operations.
/// Static methods can be invoked like
/// `IntVectorSkills::is_ascending(seq)`.
class IntVectorSkills {
public:
    // Analysis

    /// Check whether `seq` never decreases from one element to
    /// the next.
    /// @param seq the sequence to check.
    /// @return true if `seq` is sorted in non-decreasing order (an
    ///   empty or single-element sequence counts as sorted).
    static bool is_ascending(const std::vector<int>& seq);

    /// Add up every element of `seq`.
    /// @param seq the sequence to inspect.
    /// @return the sum of every element in `seq` (0 if `seq` is
    ///   empty).
    static int sum(const std::vector<int>& seq);

    /// Compute the arithmetic mean of `seq`.
    /// @param seq the sequence to inspect.
    /// @return the arithmetic mean of `seq` (0 if `seq` is empty).
    static double average(const std::vector<int>& seq);

    /// Count how many times `value` appears in `seq`.
    /// @param seq the sequence to inspect.
    /// @param value the value to count.
    /// @return how many elements of `seq` equal `value`.
    static int count(const std::vector<int>& seq, int value);

    /// Find the best-scoring contiguous run of `seq`.
    /// @param seq the sequence to inspect.
    /// @return the largest sum achievable by a contiguous, non-empty
    ///   run of elements of `seq` (0 if `seq` is empty). The run's
    ///   elements must be adjacent in `seq`, e.g. for `{2, -100, 3}`
    ///   the answer is `3`, not `5`.
    static int max_contiguous_sum(const std::vector<int>& seq);

    // Search

    /// Find the smallest value in `seq`.
    /// @param seq the sequence to inspect.
    /// @return the smallest value in `seq` (0 if `seq` is empty).
    static int min(const std::vector<int>& seq);

    /// Find the largest value in `seq`.
    /// @param seq the sequence to inspect.
    /// @return the largest value in `seq` (0 if `seq` is empty).
    static int max(const std::vector<int>& seq);

    /// Check whether `value` shows up anywhere in `seq`.
    /// @param seq the sequence to search.
    /// @param value the value to look for.
    /// @return true if `value` appears anywhere in `seq`.
    static bool contains(const std::vector<int>& seq, int value);

    /// Scan `seq` from the start to locate `value`.
    /// @param seq the sequence to search.
    /// @param value the value to look for.
    /// @return the index of the first element equal to `value`, or
    ///   -1 if `seq` does not contain it.
    static int linear_search(const std::vector<int>& seq, int value);

    /// Locate `value` in a sorted `seq` by repeatedly halving the
    /// search range.
    /// @param seq the sequence to search. Must be sorted in
    ///   ascending order (see `is_ascending`).
    /// @param value the value to look for.
    /// @return the index of `value` in `seq`, or -1 if not found.
    static int binary_search(const std::vector<int>& seq, int value);

    // Reshape

    /// Build a copy of `seq` with its elements in the opposite
    /// order.
    /// @param seq the sequence to reverse.
    /// @return a new sequence with the elements of `seq` in reverse
    ///   order.
    static std::vector<int> reverse(const std::vector<int>& seq);

    /// Build a copy of a contiguous chunk of `seq`.
    /// @param seq the sequence to slice.
    /// @param start index of the first element to include.
    /// @param stop index one past the last element to include. Must
    ///   satisfy `start <= stop <= seq.size()`.
    /// @return a new sequence with the elements of `seq` in the
    ///   half-open range `[start, stop)`.
    static std::vector<int> slice(const std::vector<int>& seq,
                                  unsigned start, unsigned stop);

    /// Join two sequences end to end into a new one.
    /// @param a the sequence whose elements come first.
    /// @param b the sequence whose elements come last.
    /// @return a new sequence with every element of `a` followed by
    ///   every element of `b`.
    static std::vector<int> concatenate(const std::vector<int>& a,
                                        const std::vector<int>& b);

    /// Build a copy of `seq` with repeated values collapsed down to
    /// their first occurrence.
    /// @param seq the sequence to inspect.
    /// @return a new sequence with the elements of `seq`, keeping
    ///   only the first occurrence of each value and preserving
    ///   their original order.
    static std::vector<int> unique(const std::vector<int>& seq);

    /// Build a copy of `seq` keeping only the elements that satisfy
    /// `criterion`, discarding the rest.
    /// @param seq the sequence to filter.
    /// @param criterion the predicate every kept element must
    ///   satisfy; elements that fail it are left out of the result.
    /// @return a new sequence with the elements of `seq` that
    ///   satisfy `criterion`, preserving their relative order.
    static std::vector<int> filter(const std::vector<int>& seq,
                                   const IntPredicate& criterion);

    /// Un-interleave `seq` into its even- and odd-position elements,
    /// without touching `seq` itself.
    /// @param seq an interleaved sequence following the pattern
    ///   `a, b, a, b, ...`.
    /// @param a set to every element of `seq` at an even index (the
    ///   `a`'s), in order.
    /// @param b set to every element of `seq` at an odd index (the
    ///   `b`'s), in order.
    static void split(const std::vector<int>& seq,
                      std::vector<int>& a,
                      std::vector<int>& b);

    // Modify: these change `seq`

    /// Strip every occurrence of `value` out of `seq`, not just the
    /// first one.
    /// @param seq the sequence to strip, in place.
    /// @param value the value to remove.
    static void remove_value(std::vector<int>& seq, int value);

    /// Insert `value` into an already-ascending `seq`, in place,
    /// keeping it ascending; if `value` already occurs in `seq`, it
    /// is inserted before the first occurrence. This could be done
    /// with `std::vector::insert`, but the point of the exercise is
    /// to build it by hand: use only `push_back` and `[]` access to
    /// find the spot and shift the elements yourself.
    /// @param seq the sequence to insert into, in place. Must
    ///   already be sorted in ascending order (see `is_ascending`).
    /// @param value the value to insert.
    static void insert_ascending(std::vector<int>& seq, int value);

    /// Replace every element of `seq` with the result of applying
    /// `operation` to it.
    /// @param seq the sequence to transform, in place.
    /// @param operation the rule applied to every element.
    static void transform(std::vector<int>& seq,
                          const IntOperation& operation);

    /// Check whether `seq` could be the degree sequence of some
    /// simple undirected graph.
    /// @param seq a candidate graph degree sequence, reduced in
    ///   place by Havel-Hakimi's greedy algorithm (left at all
    ///   zeros if `seq` is graphical; only partially reduced
    ///   otherwise).
    /// @return true if `seq` is graphical, i.e. some simple
    ///   undirected graph has exactly these vertex degrees.
    static bool havel_hakimi(std::vector<int>& seq);
};

/// A yes/no test on one element, e.g. "is it even?". There are no
/// lambdas yet, so the test is an object: derive a class from this
/// one, override `test`, then pass an instance to
/// `IntVectorSkills::filter`.
class IntPredicate {
public:
    /// Needed because there's a virtual method.
    virtual ~IntPredicate() = default;

    /// Check a single element against this predicate's rule.
    /// @param value the element to test.
    /// @return whether `value` satisfies this predicate.
    virtual bool test(int value) const = 0;
};

/// A rule that turns one element into another, e.g. doubling it.
/// There are no lambdas yet, so the rule is an object: derive a
/// class from this one, override `apply`, then pass an instance to
/// `IntVectorSkills::transform`.
class IntOperation {
public:
    /// Needed because there's a virtual method.
    virtual ~IntOperation() = default;

    /// Turn a single element into its transformed value.
    /// @param value the element to transform.
    /// @return the transformed value.
    virtual int apply(int value) const = 0;
};
