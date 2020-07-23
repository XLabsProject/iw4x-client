#include "STDInclude.hpp"

namespace Utils
{
	CSV::CSV(const std::string& file, bool isFile, bool allowComments)
	{
		this->parse(file, isFile, allowComments);
	}

	CSV::~CSV()
	{
		this->dataMap.clear();
	}

	int CSV::getRows()
	{
		return this->dataMap.size();
	}

	int CSV::getColumns(size_t row)
	{
		if (this->dataMap.size() > row)
		{
			return this->dataMap[row].size();
		}

		return 0;
	}

	int CSV::getColumns()
	{
		int count = 0;

		for (int i = 0; i < this->getRows(); ++i)
		{
			count = std::max(this->getColumns(i), count);
		}

		return count;
	}

	std::string CSV::getElementAt(size_t row, size_t column)
	{
		if (this->dataMap.size() > row)
		{
			auto _row = this->dataMap[row];

			if (_row.size() > column)
			{
				return _row[column];
			}
		}

		return "";
	}

	void CSV::parse(const std::string& file, bool isFile, bool allowComments)
	{
		std::string buffer;

		if (isFile)
		{
			if (!Utils::IO::FileExists(file)) return;
			buffer = Utils::IO::ReadFile(file);
			this->valid = true;
		}
		else
		{
			buffer = file;
		}

		if (!buffer.empty())
		{
			auto rows = Utils::String::Explode(buffer, '\n');

			for (auto& row : rows)
			{
				this->parseRow(row, allowComments);
			}
		}
	}

	void CSV::parseRow(const std::string& row, bool allowComments)
	{
		bool isString = false;
		std::string element;
		std::vector<std::string> _row;
		char tempStr = 0;

		for (unsigned int i = 0; i < row.size(); ++i)
		{
			if (row[i] == ',' && !isString) // Flush entry
			{
				_row.push_back(element);
				element.clear();
				continue;
			}
			else if (row[i] == '"') // Start/Terminate string
			{
				isString = !isString;
				continue;
			}
			else if (i < (row.size() - 1) && row[i] == '\\' &&row[i + 1] == '"' && isString) // Handle quotes in strings as \"
			{
				tempStr = '"';
				++i;
			}
			else if (!isString && (row[i] == '\n' || row[i] == '\x0D' || row[i] == '\x0A' || row[i] == '\t'))
			{
				//++i;
				continue;
			}
			else if (!isString && (row[i] == '#' || (row[i] == '/' && row[i+1] == '/') ) && allowComments) // Skip comments. I know CSVs usually don't have comments, but in this case it's useful
			{
				return;
			}
			else
			{
				tempStr = row[i];
			}

			element.append(&tempStr, 1);
		}

		// Push last element
		_row.push_back(element);

		if (_row.size() == 0 || (_row.size() == 1 && !_row[0].size())) // Skip empty rows
		{
			return;
		}

		this->dataMap.push_back(_row);
	}
}
