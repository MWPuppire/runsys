{
  "targets": [
    {
      "target_name": "runsys",
      "sources": [
        "runsys.cc"
      ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}
