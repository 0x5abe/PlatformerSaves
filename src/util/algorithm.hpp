namespace util::algorithm {
	inline int mod(int k, int n) {
		return ((k %= n) < 0) ? k+n : k;
	}

	inline unsigned int hash_string(const char * s)
	{
		unsigned int hash = 0;

		for(; *s; ++s)
		{
			hash += *s;
			hash += (hash << 10);
			hash ^= (hash >> 6);
		}

		hash += (hash << 3);
		hash ^= (hash >> 11);
		hash += (hash << 15);

		return hash;
	}
}