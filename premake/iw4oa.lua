iw4oa = {
	settings = nil,
}

function iw4oa.setup(settings)
	if not settings.source then error("Missing source.") end

	iw4oa.settings = settings
end

function iw4oa.import()
	if not iw4oa.settings then error("Run iw4oa.setup first") end

	links { "iw4oa" }
	iw4oa.includes()
end

function iw4oa.includes()
	if not iw4oa.settings then error("Run iw4oa.setup first") end

	includedirs { iw4oa.settings.source }
end

function iw4oa.project()
	if not iw4oa.settings then error("Run iw4oa.setup first") end

	project "iw4oa"
		language "C++"

		includedirs
		{
			iw4oa.settings.source,
		}

		files
		{
			path.join(iw4oa.settings.source, "**.cpp"),
			path.join(iw4oa.settings.source, "**.h"),
		}
		removefiles
		{
			path.join(iw4oa.settings.source, "test*"),
		}

		warnings "Off"

		defines { "_LIB" }
		removedefines { "_USRDLL", "_DLL" }
		kind "StaticLib"
end
