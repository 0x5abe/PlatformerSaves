namespace util::filesystem {
	inline std::string getParentDirectoryFromPath(const std::string& i_filePath)
	{
		size_t l_pos = i_filePath.find_last_of("\\/");
		return (std::string::npos == l_pos)
			? ""
			: i_filePath.substr(0, l_pos);
	}
}