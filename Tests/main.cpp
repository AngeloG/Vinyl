#include <stdio.h>

#include <Logging.h>
#include <ZipFile.h>

int main()
{
	v::Log::Level = v::Log::LogLevel_Trace;

	v::Log::Debug("Reading zip file test.zip");

	try {
		v::ZipFile zip;
		zip.Read("test.zip");

		s::StackArray<v::ZipEntry> entries;
		zip.GetEntries(entries);

		for (auto &entry : entries) {
			v::Log::Debug("Zip entry %1: %2", FVAR(entry.GetIndex()), FVAR(entry.GetName()));

			s::MemoryStream strm;
			entry.Read(strm);
			v::Log::Debug("Contents at %1 with size %2", FVAR(strm.strm_pubBuffer), FVAR(strm.Size()));
		}
	} catch (s::Exception ex) {
		v::Log::Fatal("Error: %1", FVAR(ex.Message));
	}

	getchar();
	return 0;
}
